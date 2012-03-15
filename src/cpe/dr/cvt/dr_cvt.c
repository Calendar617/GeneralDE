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

ssize_t dr_cvt_encode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    error_monitor_t em)
{
    if (cvt == NULL || output == NULL || input == NULL) return -1;
    assert(cvt->m_type);

    if (cvt->m_type->m_encode == NULL) return -1;

    return cvt->m_type->m_encode(meta, output, output_capacity, input, input_capacity, cvt->m_type->m_ctx, em);
}

ssize_t dr_cvt_decode(
    dr_cvt_t cvt, 
    LPDRMETA meta,
    void * output, size_t output_capacity,
    const void * input, size_t input_capacity,
    error_monitor_t em)
{
    if (cvt == NULL || output == NULL || input == NULL) return -1;
    assert(cvt->m_type);

    if (cvt->m_type->m_decode == NULL) return -1;

    return cvt->m_type->m_decode(meta, output, output_capacity, input, input_capacity, cvt->m_type->m_ctx, em);
}

