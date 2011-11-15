#include <string.h>
#include "yajl/yajl_parse.h"
#include "cpe/dr/dr_json.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"

#define JSON_PARSE_CTX_BUF_LEN CPE_DR_MACRO_LEN + 1

#define JSON_PARSE_CTX_COPY_STR_TMP(__ctx, str, len) do {   \
        size_t __len = len;                                 \
        if (__len >= JSON_PARSE_CTX_BUF_LEN) {              \
            __len = JSON_PARSE_CTX_BUF_LEN - 1;             \
        }                                                   \
        memcpy(__ctx->m_buf, str, __len);                   \
        __ctx->m_buf[__len] = 0;                            \
    } while(0)

struct dr_json_parse_type_info {
    LPDRMETA m_meta;
    void * m_data;
    LPDRMETAENTRY m_entry;
};

struct dr_json_parse_ctx {
    struct mem_buffer * m_output;
    error_monitor_t m_em;

    struct dr_json_parse_type_info m_typeStacks[CPE_DR_MAX_LEVEL];
    int m_typePos;

    char m_buf[JSON_PARSE_CTX_BUF_LEN]; //used to store tmp data
};

static int dr_json_null(void * ctx) {
    return 1;
}

static int dr_json_boolean(void * ctx, int boolean) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

int dr_json_do_parse_from_string(
    struct dr_json_parse_ctx * c, 
    struct dr_json_parse_type_info * parseType,
    const char * s, size_t l)
{
    const struct tagDRCTypeInfo * cTypeInfo =
        dr_find_ctype_info_by_type(parseType->m_entry->m_type);
    if (cTypeInfo == NULL) {
        CPE_ERROR(c->m_em, "internal error, no type info of type %d!", parseType->m_entry->m_type);
        return 0;
    }

    if (cTypeInfo->read_from_string) {
        JSON_PARSE_CTX_COPY_STR_TMP(c, s, l);
        if (cTypeInfo->read_from_string(
                parseType->m_data + parseType->m_entry->m_data_start_pos,
                c->m_buf) == 0)
        {
            return 1;
        }
        else {
            CPE_ERROR(c->m_em, "read %s from \"%s\" error!", cTypeInfo->m_name, c->m_buf);
            return 0;
        }
    }
    else {
        CPE_ERROR(c->m_em, "%s not support read from string!", cTypeInfo->m_name);
        return 0;
    }
}

static int dr_json_number(void * ctx, const char * s, size_t l) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    struct dr_json_parse_type_info * parseType = &c->m_typeStacks[c->m_typePos];

    if (parseType->m_entry == NULL || parseType->m_data == NULL) {
        CPE_ERROR(c->m_em, "internal error, no current entry info!");
        return 0;
    }

    return dr_json_do_parse_from_string(c, parseType, s, l);
}

static int dr_json_string(void * ctx, const unsigned char * s, size_t l) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    struct dr_json_parse_type_info * parseType = &c->m_typeStacks[c->m_typePos];

    if (parseType->m_entry == NULL || parseType->m_data == NULL) {
        CPE_ERROR(c->m_em, "internal error, no current entry info!");
        return 0;
    }

    return dr_json_do_parse_from_string(c, parseType, s, l);
}

static int dr_json_map_key(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    struct dr_json_parse_type_info * typeInfo = &c->m_typeStacks[c->m_typePos];
    LPDRMETA refType = NULL;

    JSON_PARSE_CTX_COPY_STR_TMP(c, stringVal, stringLen);

    typeInfo->m_entry = dr_meta_find_entry_by_name(typeInfo->m_meta, c->m_buf);
    if (typeInfo->m_entry == NULL) {
        CPE_INFO(c->m_em, "%s have no entry %s", dr_meta_name(typeInfo->m_meta), c->m_buf);
        return 0;
    }

    if (refType = dr_entry_ref_meta(typeInfo->m_entry)) { /*composite*/
        struct dr_json_parse_type_info * nestTypeInfo = NULL;

        if ((c->m_typePos + 1) >= CPE_DR_MAX_LEVEL) { /*nest type overflow */
            CPE_ERROR(c->m_em, "too complex, max nest level %d reached", CPE_DR_MAX_LEVEL);
            return 0;
        }

        ++c->m_typePos;
        nestTypeInfo = &c->m_typeStacks[c->m_typePos];
        nestTypeInfo->m_meta = refType;
        nestTypeInfo->m_data = typeInfo->m_data + typeInfo->m_entry->m_data_start_pos;
        nestTypeInfo->m_entry = NULL;
    }

    return 1;
}

static int dr_json_start_map(void * ctx) {
    return 1;
}

static int dr_json_end_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    if (c->m_typePos < 0) {
        CPE_ERROR(c->m_em, "internal error! type stack is empty");
        return 0;
    }
    --c->m_typePos;
    return 1;
}

static int dr_json_start_array(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static int dr_json_end_array(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static yajl_callbacks g_dr_json_callbacks = {
    dr_json_null,
    dr_json_boolean,
    NULL,
    NULL,
    dr_json_number,
    dr_json_string,
    dr_json_start_map,
    dr_json_map_key,
    dr_json_end_map,
    dr_json_start_array,
    dr_json_end_array
};

static void dr_json_parse_ctx_init(
    struct dr_json_parse_ctx * ctx,
    struct mem_buffer * buffer, 
    LPDRMETA meta,
    error_monitor_t em)
{
    ctx->m_output = buffer;

    ctx->m_typeStacks[0].m_meta = meta;
    ctx->m_typeStacks[0].m_data = mem_buffer_alloc(buffer, dr_meta_size(meta));
    ctx->m_typeStacks[0].m_entry = NULL;
    ctx->m_typePos = 0;
    ctx->m_em = em;
}

void dr_json_read_i(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta,
    error_monitor_t em)
{
    struct dr_json_parse_ctx ctx;
    yajl_handle hand;
    yajl_status stat;

    dr_json_parse_ctx_init(&ctx, result, meta, em);

    hand = yajl_alloc(&g_dr_json_callbacks, NULL, (void *)&ctx);
    if (hand == NULL) {
        CPE_ERROR_EX(em, CPE_DR_ERROR_NO_MEMORY, "can`t alloc memory for json parser");
        return;
    }

    stat = yajl_parse(hand, input, strlen(input));
    if (stat != yajl_status_ok) {  
        yajl_free(hand);
        return;
    }

    yajl_free(hand);
}

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta,
    error_monitor_t em)
{
    int ret = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        dr_json_read_i(result, input, meta, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        dr_json_read_i(result, input, meta, &logError);
    }

    return ret;
}
