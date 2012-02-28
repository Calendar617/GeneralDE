#ifndef USF_LOGIC_CONTEXT_H
#define USF_LOGIC_CONTEXT_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_context_t logic_context_create(logic_manage_t mgr, logic_require_id_t id);
void logic_context_free(logic_context_t context);

logic_context_t logic_context_find(logic_manage_t mgr, logic_context_id_t id);

logic_context_id_t
logic_context_id(logic_context_t context);

logic_context_state_t
logic_context_state(logic_context_t context);

#ifdef __cplusplus
}
#endif

#endif

