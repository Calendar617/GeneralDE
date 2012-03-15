#include "cpe/pal/pal_string.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "dr_cvt_internal_types.h"

ssize_t dr_cvt_fun_copy_encode(
    LPDRMETA meta,
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    void * ctx,
    error_monitor_t em)
{
    if (output_capacity < input_capacity) {
        CPE_ERROR(
            em, "encode %s: copy: not enought output buf, require %d, but only %d!",
            dr_meta_name(meta), (int)input_capacity, (int)output_capacity);
        return -1;
    }

    memcpy(output, input, input_capacity);
    return input_capacity;
}

ssize_t dr_cvt_fun_copy_decode(
    LPDRMETA meta,
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    void * ctx,
    error_monitor_t em)
{
    if (output_capacity < input_capacity) {
        CPE_ERROR(
            em, "decode %s: copy: not enought output buf, require %d, but only %d!",
            dr_meta_name(meta), (int)input_capacity, (int)output_capacity);
        return -1;
    }

    memcpy(output, input, input_capacity);
    return input_capacity;
}

struct dr_cvt_type s_cvt_type_copy = {
    "copy"
    , dr_cvt_fun_copy_encode
    , dr_cvt_fun_copy_decode
    , NULL
    , -1
};

