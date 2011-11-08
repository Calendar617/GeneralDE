#include <stdio.h>
#include <string.h>
#include "yajl/yajl_parse.h"
#include "cpe/dr/dr_json.h"
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
    int m_version;

    struct dr_json_parse_type_info m_typeStacks[CPE_DR_MAX_LEVEL];
    int m_typePos;

    char m_buf[JSON_PARSE_CTX_BUF_LEN]; //used to store tmp data
};

static int dr_json_null(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static int dr_json_boolean(void * ctx, int boolean) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static int dr_json_number(void * ctx, const char * s, size_t l) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    struct dr_json_parse_type_info * parseType = &c->m_typeStacks[c->m_typePos];

    if (parseType->m_entry == NULL || parseType->m_data == NULL) {
        return 0;
    }

    const struct tagDRCTypeInfo * cTypeInfo =
        dr_find_ctype_info_by_type(parseType->m_entry->m_type);
    if (cTypeInfo == NULL) {
        return 0;
    }

    if (cTypeInfo->read_from_string) {
        JSON_PARSE_CTX_COPY_STR_TMP(c, s, l);
        return 0 == cTypeInfo->read_from_string(
            parseType->m_data + parseType->m_entry->m_data_start_pos,
            c->m_buf);
    }
    else {
        return 0;
    }
}

static int dr_json_string(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static int dr_json_map_key(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    struct dr_json_parse_type_info * typeInfo = &c->m_typeStacks[c->m_typePos];
    int r;
    int entryIdx;

    JSON_PARSE_CTX_COPY_STR_TMP(c, stringVal, stringLen);

    r = dr_get_entry_by_name(&entryIdx, typeInfo->m_meta, c->m_buf);
    if (r < 0) {
        return 0;
    }

    typeInfo->m_entry = dr_get_entry_by_index(typeInfo->m_meta, entryIdx);
    if (typeInfo->m_entry == NULL) {
        return 0;
    }

    return 1;
}

static int dr_json_start_map(void * ctx) {
    return 1;
}

static int dr_json_end_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
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
    int version)
{
    ctx->m_output = buffer;
    ctx->m_version = version;

    ctx->m_typeStacks[0].m_meta = meta;
    ctx->m_typeStacks[0].m_data = mem_buffer_alloc(buffer, dr_get_meta_size(meta));
    ctx->m_typeStacks[0].m_entry = NULL;
    ctx->m_typePos = 0;
}

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETA meta, int version)
{
    struct dr_json_parse_ctx ctx;
    yajl_handle hand;
    yajl_status stat;

    dr_json_parse_ctx_init(&ctx, result, meta, version);

    hand = yajl_alloc(&g_dr_json_callbacks, NULL, (void *)&ctx);
    if (hand == NULL) {
        return -1;
    }

    stat = yajl_parse(hand, input, strlen(input));
    if (stat != yajl_status_ok) {  
        yajl_free(hand);
        return -1;
    }

    yajl_free(hand);
    return 0;
}
