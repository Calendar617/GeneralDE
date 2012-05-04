#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_stackbuf.h"
#include "cpe/utils/stream_mem.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/dr/dr_pbuf.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"
#include "dr_pbuf_internal_ops.h"

struct dr_pbuf_write_stack {
    LPDRMETA m_meta;
    LPDRMETAENTRY m_entry;
    int m_entry_pos;
    int m_entry_count;
    int m_array_pos;

    unsigned char * m_output_data;
    size_t m_output_size;
    size_t m_output_capacity;

    size_t m_array_begin_pos;

    char const * m_input_data;
    size_t m_input_data_capacity;
};

#define dr_pbuf_write_size_reserve 1

#define dr_pbuf_write_check_capacity(__capacity)                        \
    if (curStack->m_output_capacity - curStack->m_output_size < (__capacity)) { \
        CPE_ERROR(em, "dr_pbuf_write: not enouth buf!");                \
        return -1;                                                      \
    }

#define dr_pbuf_write_encode_uint32(v)                              \
    dr_pbuf_write_check_capacity(10);                               \
    curStack->m_output_size +=                                      \
        cpe_dr_pbuf_encode32(                                       \
            v, curStack->m_output_data + curStack->m_output_size)

#define dr_pbuf_write_encode_int32(v)                   \
    dr_pbuf_write_check_capacity(10);                               \
    curStack->m_output_size +=                                      \
        cpe_dr_pbuf_zigzag32(                                       \
            v, curStack->m_output_data + curStack->m_output_size)

#define dr_pbuf_write_encode_uint64(v)                              \
    dr_pbuf_write_check_capacity(10);                               \
    curStack->m_output_size +=                                      \
        cpe_dr_pbuf_encode64(                                       \
            v, curStack->m_output_data + curStack->m_output_size)

#define dr_pbuf_write_encode_int64(v)                   \
    dr_pbuf_write_check_capacity(10);                               \
    curStack->m_output_size +=                                      \
        cpe_dr_pbuf_zigzag64(                                       \
            v, curStack->m_output_data + curStack->m_output_size)

#define dr_pbuf_write_encode_id_and_type(t) dr_pbuf_write_encode_uint32((((uint32_t)curStack->m_entry->m_id) << 3) | (t));

int dr_pbuf_write(
    void * output,
    size_t output_capacity,
    const void * input,
    size_t input_capacity,
    LPDRMETA meta,
    error_monitor_t em)
{
    struct dr_pbuf_write_stack processStack[CPE_DR_MAX_LEVEL];
    int stackPos;

    assert(output);
    assert(input);
    assert(meta);

    processStack[0].m_meta = meta;
    processStack[0].m_entry = dr_meta_entry_at(meta, 0);
    processStack[0].m_entry_pos = 0;
    processStack[0].m_entry_count = meta->m_entry_count;
    processStack[0].m_array_pos = 0;
    processStack[0].m_output_data = (unsigned char *)output;
    processStack[0].m_output_size = 0;
    processStack[0].m_output_capacity = output_capacity;
    processStack[0].m_array_begin_pos = 0;
    processStack[0].m_input_data = (char const *)input;
    processStack[0].m_input_data_capacity = input_capacity;

    for(stackPos = 0; stackPos >= 0;) {
        struct dr_pbuf_write_stack * curStack;

        assert(stackPos < CPE_DR_MAX_LEVEL);

        curStack = &processStack[stackPos];
        if (curStack->m_meta == NULL) {
            --stackPos;
            continue;
        }

        for(; curStack->m_entry_pos < curStack->m_entry_count
                && curStack->m_entry;
            ++curStack->m_entry_pos
                , curStack->m_array_pos = 0
                , curStack->m_entry = dr_meta_entry_at(curStack->m_meta, curStack->m_entry_pos)
            )
        {
            size_t elementSize;
            int32_t array_count;
            LPDRMETAENTRY refer;

        LOOPENTRY:
            if (curStack->m_entry->m_id == -1) continue; /*ignore no id field*/

            elementSize = dr_entry_element_size(curStack->m_entry);
            if (elementSize == 0) continue;

            refer = NULL;
            if (curStack->m_entry->m_array_count != 1) {
                refer = dr_entry_array_refer_entry(curStack->m_entry);
            }

            array_count = curStack->m_entry->m_array_count;
            if (refer) {
                dr_entry_try_read_int32(
                    &array_count,
                    curStack->m_input_data + curStack->m_entry->m_array_refer_data_start_pos,
                    refer,
                    em);
            }

            for(; curStack->m_array_pos < array_count; ++curStack->m_array_pos) {
                const char * entryData;

                if (curStack->m_entry->m_array_count != 1
                    && curStack->m_array_pos == 0
                    && (curStack->m_entry->m_type != CPE_DR_TYPE_UNION
                        && curStack->m_entry->m_type != CPE_DR_TYPE_STRUCT
                        && curStack->m_entry->m_type != CPE_DR_TYPE_STRING))
                {
                    dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_LENGTH);

                    curStack->m_array_begin_pos = curStack->m_output_size;
                    dr_pbuf_write_check_capacity(curStack->m_output_size + dr_pbuf_write_size_reserve);
                    curStack->m_output_size += dr_pbuf_write_size_reserve;
                }

                entryData = curStack->m_input_data + curStack->m_entry->m_data_start_pos + (elementSize * curStack->m_array_pos);

                switch(curStack->m_entry->m_type) {
                case CPE_DR_TYPE_UNION:
                case CPE_DR_TYPE_STRUCT: {
                    dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_LENGTH);

                    if (stackPos + 1 < CPE_DR_MAX_LEVEL) {
                        struct dr_pbuf_write_stack * nextStack;
                        nextStack = &processStack[stackPos + 1];
                        nextStack->m_meta = dr_entry_ref_meta(curStack->m_entry);
                        if (nextStack->m_meta == 0) break;

                        nextStack->m_input_data = entryData;
                        nextStack->m_input_data_capacity = elementSize;

                        nextStack->m_output_data = curStack->m_output_data + curStack->m_output_size + dr_pbuf_write_size_reserve;
                        nextStack->m_output_size = 0;
                        nextStack->m_output_capacity = curStack->m_output_capacity - curStack->m_output_size;
                        nextStack->m_array_begin_pos = 0;

                        nextStack->m_entry_pos = 0;
                        nextStack->m_entry_count = nextStack->m_meta->m_entry_count;

                        if (curStack->m_entry->m_type == CPE_DR_TYPE_UNION) {
                            LPDRMETAENTRY select_entry;
                            select_entry = dr_entry_select_entry(curStack->m_entry);
                            if (select_entry) {
                                int32_t union_entry_id;
                                dr_entry_try_read_int32(
                                    &union_entry_id,
                                    curStack->m_input_data + curStack->m_entry->m_select_data_start_pos,
                                    select_entry,
                                    em);
                                
                                nextStack->m_entry_pos =
                                    dr_meta_find_entry_idx_by_id(nextStack->m_meta, union_entry_id);
                                if (nextStack->m_entry_pos < 0) continue;

                                nextStack->m_entry_count = nextStack->m_entry_pos + 1;
                            }
                        }

                        nextStack->m_entry = dr_meta_entry_at(nextStack->m_meta, nextStack->m_entry_pos);

                        nextStack->m_array_pos = 0;
                        ++curStack->m_array_pos;
                        ++stackPos;
                        curStack = nextStack;
                        goto LOOPENTRY;
                    }
                    break;
                }
                case CPE_DR_TYPE_CHAR:
                case CPE_DR_TYPE_INT8:
                case CPE_DR_TYPE_INT16:
                case CPE_DR_TYPE_INT32: {
                    int32_t value;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_VARINT);
                    }

                    dr_entry_try_read_int32(&value, entryData, curStack->m_entry, NULL);
                    dr_pbuf_write_encode_int32(value);
                    break;
                }
                case CPE_DR_TYPE_INT64: {
                    int64_t value;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_VARINT);
                    }

                    dr_entry_try_read_int64(&value, entryData, curStack->m_entry, NULL);
                    dr_pbuf_write_encode_int64(value);
                    break;
                }
                case CPE_DR_TYPE_UCHAR:
                case CPE_DR_TYPE_UINT8:
                case CPE_DR_TYPE_UINT16:
                case CPE_DR_TYPE_UINT32: {
                    uint32_t value;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_VARINT);
                    }

                    dr_entry_try_read_uint32(&value, entryData, curStack->m_entry, NULL);
                    dr_pbuf_write_encode_uint32(value);
                    break;
                }
                case CPE_DR_TYPE_UINT64: {
                    uint64_t value;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_VARINT);
                    }

                    dr_entry_try_read_uint64(&value, entryData, curStack->m_entry, NULL);
                    dr_pbuf_write_encode_uint64(value);
                    break;
                }
                case CPE_DR_TYPE_FLOAT: {
                    union {
                        float v;
                        uint32_t e;
                    } u;
                    unsigned char * buffer;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_32BIT);
                    }

                    dr_pbuf_write_check_capacity(curStack->m_output_size + 4);

                    dr_entry_try_read_float(&u.v, entryData, curStack->m_entry, NULL);

                    buffer = curStack->m_output_data + curStack->m_output_size;

                    buffer[0] = (uint8_t) (u.e & 0xff);
                    buffer[1] = (uint8_t) (u.e >> 8 & 0xff);
                    buffer[2] = (uint8_t) (u.e >> 16 & 0xff);
                    buffer[3] = (uint8_t) (u.e >> 24 & 0xff);

                    curStack->m_output_size += 4;

                    break;
                }
                case CPE_DR_TYPE_DOUBLE: {
                    union {
                        double v;
                        uint64_t e;
                    } u;
                    unsigned char * buffer;

                    if (curStack->m_entry->m_array_count == 1) {
                        dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_64BIT);
                    }

                    dr_pbuf_write_check_capacity(curStack->m_output_size + 8);

                    dr_entry_try_read_double(&u.v, entryData, curStack->m_entry, NULL);

                    buffer = curStack->m_output_data + curStack->m_output_size;
                    buffer[0] = (uint8_t) (u.e & 0xff);
                    buffer[1] = (uint8_t) (u.e >> 8 & 0xff);
                    buffer[2] = (uint8_t) (u.e >> 16 & 0xff);
                    buffer[3] = (uint8_t) (u.e >> 24 & 0xff);
                    buffer[4] = (uint8_t) (u.e >> 32 & 0xff);
                    buffer[5] = (uint8_t) (u.e >> 40 & 0xff);
                    buffer[6] = (uint8_t) (u.e >> 48 & 0xff);
                    buffer[7] = (uint8_t) (u.e >> 56 & 0xff);

                    curStack->m_output_size += 8;

                    break;
                }
                case CPE_DR_TYPE_STRING: {
                    uint32_t len;

                    len = strlen((const char *)entryData);

                    dr_pbuf_write_encode_id_and_type(CPE_PBUF_TYPE_LENGTH);
                    dr_pbuf_write_encode_uint32(len);
                    dr_pbuf_write_check_capacity(len);
                    memcpy(curStack->m_output_data + curStack->m_output_size, entryData, len);
                    curStack->m_output_size += len;
                    break;
                }
                case CPE_DR_TYPE_DATE:
                case CPE_DR_TYPE_TIME:
                case CPE_DR_TYPE_DATETIME:
                case CPE_DR_TYPE_MONEY:
                case CPE_DR_TYPE_IP:
                case CPE_DR_TYPE_VOID:
                default:
                    break;
                }
            }

            if (curStack->m_entry->m_array_count != 1
                && (curStack->m_entry->m_type != CPE_DR_TYPE_UNION
                    && curStack->m_entry->m_type != CPE_DR_TYPE_STRUCT
                    && curStack->m_entry->m_type != CPE_DR_TYPE_STRING))
            {
                unsigned char size_buf[10];
                size_t len;
                size_t total;
                int size_size;

                len = curStack->m_output_size - curStack->m_array_begin_pos - dr_pbuf_write_size_reserve;
                size_size = cpe_dr_pbuf_encode32(len, size_buf);
                total = curStack->m_array_begin_pos + size_size + len;

                memmove(
                    curStack->m_output_data + curStack->m_array_begin_pos + size_size,
                    curStack->m_output_data + curStack->m_array_begin_pos + dr_pbuf_write_size_reserve,
                    len);

                memcpy(curStack->m_output_data + curStack->m_array_begin_pos, size_buf, size_size);

                curStack->m_output_size = total;
            }
        }

        if (--stackPos >= 0) {
            struct dr_pbuf_write_stack * preStack;
            unsigned char size_buf[10];
            int size_size;

            preStack = &processStack[stackPos];

            size_size = cpe_dr_pbuf_encode32(curStack->m_output_size, size_buf);

            memmove(
                preStack->m_output_data + preStack->m_output_size + size_size,
                curStack->m_output_data,
                curStack->m_output_size);

            memcpy(preStack->m_output_data + preStack->m_output_size, size_buf, size_size);

            preStack->m_output_size += (size_size + curStack->m_output_size); 
        }
    }

    return (int)processStack[0].m_output_size;
}
