#ifndef USF_LOGIC_REQUIRE_H
#define USF_LOGIC_REQUIRE_H
#include "cpe/utils/hash_string.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_require_t
logic_require_create(logic_context_t context, cpe_hash_string_t require_name);
void logic_require_free(logic_require_t require);

logic_require_t
logic_require_find(logic_manage_t mgr, logic_require_id_t id);

logic_require_state_t
logic_require_state(logic_require_t require);

void logic_require_set_done(logic_require_t require);
void logic_require_set_error(logic_require_t require);

#ifdef __cplusplus
}
#endif

#endif

