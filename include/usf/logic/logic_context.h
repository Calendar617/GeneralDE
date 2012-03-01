#ifndef USF_LOGIC_CONTEXT_H
#define USF_LOGIC_CONTEXT_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_context_t
logic_context_create(
    logic_manage_t mgr,
    logic_require_id_t id,
    logic_executor_t executor,
    size_t capacity);

void logic_context_free(logic_context_t context);

logic_context_t logic_context_find(logic_manage_t mgr, logic_context_id_t id);

logic_context_id_t logic_context_id(logic_context_t context);
logic_context_state_t logic_context_state(logic_context_t context);
logic_manage_t logic_context_mgr(logic_context_t context);
gd_app_context_t logic_context_app(logic_context_t context);
int32_t logic_context_errno(logic_context_t context);
size_t logic_context_capacity(logic_context_t context);
void * logic_context_data(logic_context_t context);

uint32_t logic_context_flags(logic_context_t context);
void logic_context_flags_set(logic_context_t context, uint32_t flag);
void logic_context_flag_enable(logic_context_t context, logic_context_flag_t flag);
void logic_context_flag_disable(logic_context_t context, logic_context_flag_t flag);
int logic_context_flag_is_enable(logic_context_t context, logic_context_flag_t flag);

void logic_context_execute(logic_context_t context);

#ifdef __cplusplus
}
#endif

#endif

