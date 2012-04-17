#ifndef CPE_DR_CVT_INTERNALS_TYPES_H
#define CPE_DR_CVT_INTERNALS_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dr_cvt_type {
    const char * m_name;
    dr_cvt_fun_t m_encode;
    dr_cvt_fun_t m_decode;
    void * m_ctx;
    int m_ref_count;

    TAILQ_ENTRY(dr_cvt_type) m_next;
};

typedef TAILQ_HEAD(dr_cvt_type_list, dr_cvt_type) dr_cvt_type_list_t;

struct dr_cvt {
    struct dr_cvt_type * m_type;
};

#ifdef __cplusplus
}
#endif

#endif
