#include <assert.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_pbuf.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"
#include "dr_pbuf_internal_ops.h"

struct dr_pbuf_read_array_info {
    uint32_t m_entry_id;
    uint32_t m_count;
};

struct dr_pbuf_read_stack {
    LPDRMETA m_meta;
    size_t m_output_start_pos;
    uint8_t const * m_input_data;
    size_t m_input_size;
    size_t m_input_capacity;

    struct dr_pbuf_read_array_info m_array_infos[20];
};

struct dr_pbuf_read_ctx {
    char * m_output_buf;
    size_t m_output_capacity;
    struct mem_buffer * m_output_alloc;
    size_t m_used_size;
    error_monitor_t m_em;
};

inline static
struct dr_pbuf_read_array_info *
dr_pbuf_read_get_array_info(
    struct dr_pbuf_read_stack * stackInfo,
    uint32_t entityId)
{
    int pos;
    for(pos = 0;
        pos < sizeof(stackInfo->m_array_infos) / sizeof(stackInfo->m_array_infos[0]);
        ++pos)
    {
        if (stackInfo->m_array_infos[pos].m_entry_id == entityId) {
            return &stackInfo->m_array_infos[pos];
        }
    }

    if (pos >= sizeof(stackInfo->m_array_infos) / sizeof(stackInfo->m_array_infos[0])) {
        return NULL; /*overflow*/
    }

    stackInfo->m_array_infos[pos].m_entry_id = entityId;
    return &stackInfo->m_array_infos[pos];
}

inline static char * dr_pbuf_read_get_write_pos(
    struct dr_pbuf_read_ctx * ctx,
    struct dr_pbuf_read_stack * stackInfo,
    int start_pos,
    size_t capacity)
{
    size_t start;
    size_t total_size;

    if (start_pos < 0) return NULL;

    assert(ctx);
    assert(stackInfo);

    start = stackInfo->m_output_start_pos + start_pos;
    total_size = start + capacity;

    if (ctx->m_output_buf) {
        if (total_size > ctx->m_output_capacity) return NULL;

        if (total_size > ctx->m_used_size) ctx->m_used_size = total_size;

        return ctx->m_output_buf + start;
    }
    else {
        assert(ctx->m_output_alloc);

        if (mem_buffer_size(ctx->m_output_alloc) < total_size) {
            if (mem_buffer_set_size(ctx->m_output_alloc, total_size) != 0) return NULL;
        }

        assert(total_size <= mem_buffer_size(ctx->m_output_alloc));

        if (total_size > ctx->m_used_size) ctx->m_used_size = total_size;

        return mem_buffer_at(ctx->m_output_alloc, start);
    }
}

inline static char * dr_pbuf_read_get_read_pos(
    struct dr_pbuf_read_ctx * ctx,
    struct dr_pbuf_read_stack * stackInfo,
    int start_pos,
    size_t capacity)
{
    size_t start;
    size_t total_size;

    if (start_pos < 0) return NULL;

    assert(ctx);
    assert(stackInfo);

    start = stackInfo->m_output_start_pos + start_pos;
    total_size = start + capacity;

    if (total_size > ctx->m_used_size) return NULL;

    if (ctx->m_output_buf) {
        return ctx->m_output_buf + start;
    }
    else {
        return mem_buffer_at(ctx->m_output_alloc, start);
    }
}

#define dr_pbuf_read_stack_init(                            \
    __stackInfo, __start_pos,                               \
    __meta, __input, __input_capacity)                      \
    do {                                                    \
        bzero(__stackInfo, sizeof(*__stackInfo));           \
        (__stackInfo)->m_meta = __meta;                     \
        (__stackInfo)->m_output_start_pos = __start_pos;    \
        (__stackInfo)->m_input_data = __input;              \
        (__stackInfo)->m_input_size = 0;                    \
        (__stackInfo)->m_input_capacity = __input_capacity; \
    } while(0)

#define dr_pbuf_read_check_capacity(__capacity)                         \
    if (curStack->m_input_capacity - curStack->m_input_size < (__capacity)) { \
        CPE_ERROR(em, "dr_pbuf_read: not enouth buf!");                 \
        return -1;                                                      \
    }

#define dr_pbuf_read_decode_varint(v) do {                         \
        int r = cpe_dr_pbuf_decode(                                 \
            curStack->m_input_data + curStack->m_input_size, &v);   \
        dr_pbuf_read_check_capacity(r);                             \
        curStack->m_input_size += r;                                \
    } while(0)

#define dr_pbuf_read_ignore_value(t)            \
    switch(t) {                                 \
    case CPE_PBUF_TYPE_VARINT: {                \
        struct cpe_dr_pbuf_longlong rb;         \
        dr_pbuf_read_decode_varint(rb);         \
        break;                                  \
    }                                           \
    case CPE_PBUF_TYPE_64BIT:                   \
        dr_pbuf_read_check_capacity(8);         \
        curStack->m_input_size += 8;            \
        break;                                  \
    case CPE_PBUF_TYPE_LENGTH: {                \
        struct cpe_dr_pbuf_longlong rb;         \
        dr_pbuf_read_decode_varint(rb);         \
        dr_pbuf_read_check_capacity(rb.low);    \
        curStack->m_input_size += rb.low;       \
        break;                                  \
    }                                           \
    case CPE_PBUF_TYPE_32BIT:                   \
        dr_pbuf_read_check_capacity(4);         \
        curStack->m_input_size += 4;            \
        break;                                  \
    }                                           \

static int dr_pbuf_read_i(
    void * output,
    size_t output_capacity,
    struct mem_buffer * result_buf, 
    const void * input,
    size_t input_capacity,
    LPDRMETA meta,
    error_monitor_t em)
{
    struct dr_pbuf_read_ctx ctx;
    struct dr_pbuf_read_stack processStack[CPE_DR_MAX_LEVEL];
    struct cpe_dr_pbuf_longlong buf_i;
    int stackPos;

    assert(input);
    assert(meta);

    ctx.m_output_buf = output;
    ctx.m_output_capacity = output_capacity;
    ctx.m_output_alloc = result_buf;
    ctx.m_em = em;
    ctx.m_used_size = 0;

    dr_pbuf_read_stack_init(
        &processStack[0], 0, meta, input, input_capacity);

    for(stackPos = 0; stackPos >= 0;) {
        struct dr_pbuf_read_stack * curStack;

        assert(stackPos < CPE_DR_MAX_LEVEL);

        curStack = &processStack[stackPos];
        if (curStack->m_meta == NULL) {
            --stackPos;
            continue;
        }

        while(curStack->m_input_size < curStack->m_input_capacity) {
            uint32_t entry_id;
            uint32_t value_type;
            int entry_pos;
            LPDRMETAENTRY entry;
            struct dr_pbuf_read_array_info * array_info;
            size_t elementSize;
            size_t writePos;
            char * writeBuf;

            dr_pbuf_read_decode_varint(buf_i);
            entry_id = buf_i.low >> 3;
            value_type = buf_i.low & 0x7;

            /*find entry*/
            entry_pos = dr_meta_find_entry_idx_by_id(curStack->m_meta, (int)entry_id);
            if (entry_pos < 0) goto DR_PBUF_READ_IGNORE;

            entry = dr_meta_entry_at(curStack->m_meta, entry_pos);
            if (entry == NULL) goto DR_PBUF_READ_IGNORE;

            /*get write pos*/
            elementSize = dr_entry_element_size(entry);

            writePos = entry->m_data_start_pos;

            array_info = NULL;
            if (entry->m_array_count != 1) {
                array_info = dr_pbuf_read_get_array_info(curStack, entry->m_id);
                if (array_info == NULL) goto DR_PBUF_READ_IGNORE;

                writePos += elementSize * array_info->m_count;
            }

            writeBuf = dr_pbuf_read_get_write_pos(&ctx, curStack, writePos, elementSize);
            if (writeBuf == NULL) goto DR_PBUF_READ_IGNORE;

            switch(entry->m_type) {
            case CPE_DR_TYPE_UNION:
            case CPE_DR_TYPE_STRUCT: {
                goto DR_PBUF_READ_IGNORE;
                break;
            }
            case CPE_DR_TYPE_CHAR:
            case CPE_DR_TYPE_INT8:
            case CPE_DR_TYPE_INT16:
            case CPE_DR_TYPE_INT32: {
                dr_pbuf_read_decode_varint(buf_i);
                cpe_dr_pbuf_dezigzag32(&buf_i);
                dr_entry_set_from_uint32(writeBuf, buf_i.low, entry, em);
                break;
            }
            case CPE_DR_TYPE_INT64: {
                break;
            }
            case CPE_DR_TYPE_UCHAR:
            case CPE_DR_TYPE_UINT8:
            case CPE_DR_TYPE_UINT16:
            case CPE_DR_TYPE_UINT32: {
                dr_pbuf_read_decode_varint(buf_i);
                dr_entry_set_from_uint32(writeBuf, buf_i.low, entry, em);
                break;
            }
            case CPE_DR_TYPE_UINT64: {
                dr_pbuf_read_decode_varint(buf_i);
                dr_entry_set_from_uint32(writeBuf, buf_i.low, entry, em);
                break;
            }
            case CPE_DR_TYPE_FLOAT: {
                break;
            }
            case CPE_DR_TYPE_DOUBLE: {
                break;
            }
            case CPE_DR_TYPE_STRING: {
                break;
            }
            case CPE_DR_TYPE_DATE:
            case CPE_DR_TYPE_TIME:
            case CPE_DR_TYPE_DATETIME:
            case CPE_DR_TYPE_MONEY:
            case CPE_DR_TYPE_IP:
            case CPE_DR_TYPE_VOID:
            default:
                goto DR_PBUF_READ_IGNORE;
            }

            continue;
        DR_PBUF_READ_IGNORE:
            dr_pbuf_read_ignore_value(value_type);
        }

        --stackPos;
    }

    return (int)ctx.m_used_size;
}

int dr_pbuf_read(
    void * output,
    size_t output_capacity,
    const void * input,
    size_t input_capacity,
    LPDRMETA meta,
    error_monitor_t em)
{
    int ret = 0;
    int size = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        size = dr_pbuf_read_i(output, output_capacity, NULL, input, input_capacity, meta, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        size = dr_pbuf_read_i(output, output_capacity, NULL, input, input_capacity, meta, &logError);
    }

    return ret == 0 ? size : ret;
}

int dr_pbuf_read_to_buffer(
    struct mem_buffer * result, 
    const void * input,
    size_t input_capacity,
    LPDRMETA meta,
    error_monitor_t em)
{
    int ret = 0;
    int size = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        size = dr_pbuf_read_i(NULL, 0, result, input, input_capacity, meta, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        size = dr_pbuf_read_i(NULL, 0, result, input, input_capacity, meta, &logError);
    }

    return ret == 0 ? size : ret;
}
