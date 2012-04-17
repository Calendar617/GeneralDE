#ifndef CPE_DR_CVT_INTERNALS_OPS_H
#define CPE_DR_CVT_INTERNALS_OPS_H
#include "dr_cvt_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dr_cvt_type * dr_cvt_type_find(const char * name);
void dr_cvt_type_add_ref(struct dr_cvt_type * type);
void dr_cvt_type_remove_ref(struct dr_cvt_type * type);

#ifdef __cplusplus
}
#endif

#endif
