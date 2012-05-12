#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_stdio.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_bson.h"
#include "cpe/dr/dr_cvt.h"

dr_cvt_result_t dr_cvt_fun_bson_encode(
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    void * ctx,
    error_monitor_t em, int debug)
{
    int r;

    r = dr_bson_write(output, *output_capacity, input, *input_capacity, meta, em);
    if (r < 0) {
        CPE_ERROR(
            em, "encode %s: bson: fail, input buf "FMT_SIZE_T", output buf "FMT_SIZE_T,
            dr_meta_name(meta), *input_capacity, *output_capacity);
        return dr_cvt_result_error;
    }

    *output_capacity = r;

    if (debug) {
        CPE_INFO(
            em, "encode %s: bson: ok, %d data to output, input-size="FMT_SIZE_T,
            dr_meta_name(meta), r, *input_capacity);
    }

    return dr_cvt_result_success;
}

dr_cvt_result_t
dr_cvt_fun_bson_decode(
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    void * ctx,
    error_monitor_t em, int debug)
{
    int r;

    r = dr_bson_read(output, *output_capacity, input, *input_capacity, meta, em);
    if (r < 0) {
        CPE_ERROR(
            em, "decode %s: bson: fail, input buf "FMT_SIZE_T", output buf "FMT_SIZE_T,
            dr_meta_name(meta), *input_capacity, *output_capacity);
        return dr_cvt_result_error;
    }

    if (debug) {
        CPE_INFO(
            em, "decode %s: bson: ok, %d data to output, input-size="FMT_SIZE_T,
            dr_meta_name(meta), r, *input_capacity);
    }

    return dr_cvt_result_success;
}

EXPORT_DIRECTIVE
int cpe_dr_bson_cvt_global_init() {
    return dr_cvt_type_create("bson", dr_cvt_fun_bson_encode, dr_cvt_fun_bson_decode, NULL);
}

EXPORT_DIRECTIVE
void cpe_dr_bson_cvt_global_fini() {
    dr_cvt_type_free("bson");
}
