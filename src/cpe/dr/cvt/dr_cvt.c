#include <assert.h>
#include "cpe/utils/memory.h"
#include "dr_cvt_internal_ops.h"

dr_cvt_t dr_cvt_create(const char * name) {
    struct dr_cvt_type * type;
    dr_cvt_t cvt;

    type = dr_cvt_type_find(name);
    if (type == NULL) return NULL;

    cvt = mem_alloc(NULL, sizeof(struct dr_cvt));
    if (cvt == NULL) return NULL;

    cvt->m_type = type;
    dr_cvt_type_add_ref(type);

    return cvt;
}

void dr_cvt_free(dr_cvt_t cvt) {
    assert(cvt);
    assert(cvt->m_type);

    dr_cvt_type_remove_ref(cvt->m_type);
    mem_free(NULL, cvt);
}

const char * dr_cvt_name(dr_cvt_t cvt) {
    return cvt->m_type->m_name;
}

dr_cvt_result_t dr_cvt_encode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    error_monitor_t em, int debug)
{
    if (cvt == NULL) {
        CPE_ERROR(em, "dr_cvt_encode: input cvt is null!");
        return dr_cvt_result_error;
    }

    if (meta == NULL) {
        CPE_ERROR(em, "dr_cvt_encode: input meta is null!");
        return dr_cvt_result_error;
    }

    if (output == NULL) {
        CPE_ERROR(em, "dr_cvt_encode: output buf is null!");
        return dr_cvt_result_error;
    }

    if (input == NULL) {
        CPE_ERROR(em, "dr_cvt_encode: input buf is null!");
        return dr_cvt_result_error;
    }

    assert(cvt->m_type);

    if (cvt->m_type->m_encode == NULL) {
        CPE_ERROR(em, "dr_cvt_encode: cvt %s not support encode!", cvt->m_type->m_name);
        return dr_cvt_result_error;
    }

    return cvt->m_type->m_encode(meta, output, output_capacity, input, input_capacity, cvt->m_type->m_ctx, em, debug);
}

dr_cvt_result_t dr_cvt_decode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t * output_capacity,
    const void * input, size_t * input_capacity,
    error_monitor_t em, int debug)
{
    if (cvt == NULL) {
        CPE_ERROR(em, "dr_cvt_decode: input cvt is null!");
        return dr_cvt_result_error;
    }

    if (meta == NULL) {
        CPE_ERROR(em, "dr_cvt_decode: input meta is null!");
        return dr_cvt_result_error;
    }

    if (output == NULL) {
        CPE_ERROR(em, "dr_cvt_decode: output buf is null!");
        return dr_cvt_result_error;
    }

    if (input == NULL) {
        CPE_ERROR(em, "dr_cvt_decode: input buf is null!");
        return dr_cvt_result_error;
    }

    assert(cvt->m_type);

    if (cvt->m_type->m_decode == NULL) {
        CPE_ERROR(em, "dr_cvt_decode: cvt %s not support decode!", cvt->m_type->m_name);
        return -1;
    }

    return cvt->m_type->m_decode(meta, output, output_capacity, input, input_capacity, cvt->m_type->m_ctx, em, debug);
}

