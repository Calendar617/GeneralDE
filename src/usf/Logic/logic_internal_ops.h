#ifndef USF_LOGIC_INTERNAL_OPS_H
#define USF_LOGIC_INTERNAL_OPS_H
#include "logic_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*context ops*/
uint32_t logic_context_hash(const struct logic_context * context);
int logic_context_cmp(const struct logic_context * l, const struct logic_context * r);

/*require ops*/
uint32_t logic_require_hash(const struct logic_require * require);
int logic_require_cmp(const struct logic_require * l, const struct logic_require * r);

/*require type ops*/
uint32_t logic_require_type_hash(const struct logic_require_type * require);
int logic_require_type_cmp(const struct logic_require_type * l, const struct logic_require_type * r);

#ifdef __cplusplus
}
#endif

#endif
