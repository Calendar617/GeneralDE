#ifndef USF_LOGIC_REQUIRE_TYPE_H
#define USF_LOGIC_REQUIRE_TYPE_H
#include "cpe/utils/hash_string.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_require_type_t
logic_require_type_create(logic_manage_t mgr, const char * name);
void logic_require_type_free(logic_require_type_t rt);

logic_require_type_t
logic_require_type_find(logic_manage_t mgr, cpe_hash_string_t type_name);

void logic_require_type_set_destory(logic_require_type_t rt, logic_require_type_trigger_t op, void * user_data);
void logic_require_type_set_cancel(logic_require_type_t rt, logic_require_type_trigger_t op, void * user_data);

#ifdef __cplusplus
}
#endif

#endif

