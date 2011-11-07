#include <stdio.h>
#include <string.h>
#include "yajl/yajl_parse.h"
#include "cpe/dr/dr_json.h"

struct dr_json_parse_type_info {
    LPDRMETA m_meta;
    void * m_data_base;
};

struct dr_json_parse_ctx {
    struct mem_buffer * m_output;
    int m_version;

    struct dr_json_parse_type_info m_typeStacks[CPE_DR_MAX_LEVEL];
    int m_type_pos;

    void * m_currentData;
    LPDRMETAENTRY m_currentEntry;
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
    return 1;
}

static int dr_json_string(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 1;
}

static int dr_json_map_key(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    printf("mape-key\n");
    return 1;
}

static int dr_json_start_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    printf("start: \n");
    return 1;
}

static int dr_json_end_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    printf("end: \n");
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
    ctx->m_typeStacks[0].m_data_base = 0; /*TODO: */
    ctx->m_type_pos = 0;

    ctx->m_currentData = NULL;
    ctx->m_currentEntry = NULL;
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
