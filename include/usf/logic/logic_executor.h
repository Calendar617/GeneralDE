#ifndef USF_LOGIC_OP_EXECUTOR_H
#define USF_LOGIC_OP_EXECUTOR_H
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_executor_t
logic_executor_basic_create(logic_manage_t mgr, logic_op_fun_t op, void * op_ctx, cfg_t args);

logic_executor_t
logic_executor_group_create(logic_manage_t mgr);
int logic_executor_group_add(logic_executor_t group, logic_executor_t member);

logic_executor_t
logic_executor_decorate_create(logic_manage_t mgr, logic_decorate_fun_t op, void * op_ctx, logic_executor_t inner);
int logic_executor_decorate_exec_inner(logic_executor_t executor, logic_context_t ctx);

void logic_executor_free(logic_executor_t executor);

#ifdef __cplusplus
}
#endif

#endif

