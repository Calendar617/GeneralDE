#include <string.h>
#include "yajl/yajl_parse.h"
#include "cpe/dr/dr_json.h"

struct dr_json_parse_ctx {
    struct mem_buffer * m_output;
};

static int dr_json_null(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_boolean(void * ctx, int boolean) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_number(void * ctx, const char * s, size_t l) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_string(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_map_key(void * ctx, const unsigned char * stringVal, size_t stringLen) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_start_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_end_map(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_start_array(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
}

static int dr_json_end_array(void * ctx) {
    struct dr_json_parse_ctx * c = (struct dr_json_parse_ctx *) ctx;
    return 0;
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

int dr_json_read(
    struct mem_buffer * result, 
    const char * input,
    LPDRMETALIB metaLib, int version)
{
    struct dr_json_parse_ctx ctx;
    yajl_handle hand;
    yajl_status stat;

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
