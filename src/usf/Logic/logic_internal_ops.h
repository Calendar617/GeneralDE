#ifndef USF_LOGIC_INTERNAL_OPS_H
#define USF_LOGIC_INTERNAL_OPS_H
#include "logic_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*context ops*/
uint32_t logic_context_hash(const struct logic_context * context);
int logic_context_cmp(const struct logic_context * l, const struct logic_context * r);
void logic_context_free_all(logic_manage_t mgr);

/*require ops*/
uint32_t logic_require_hash(const struct logic_require * require);
int logic_require_cmp(const struct logic_require * l, const struct logic_require * r);
void logic_require_free_all(logic_manage_t mgr);

/*data ops*/
uint32_t logic_data_hash(const struct logic_data * data);
int logic_data_cmp(const struct logic_data * l, const struct logic_data * r);
void logic_data_free_all(logic_manage_t mgr);

/*require type ops*/
uint32_t logic_require_type_hash(const struct logic_require_type * require);
int logic_require_type_cmp(const struct logic_require_type * l, const struct logic_require_type * r);
void logic_require_type_free_all(logic_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
