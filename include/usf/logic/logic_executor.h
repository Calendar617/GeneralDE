#ifndef USF_LOGIC_OP_EXECUTOR_H
#define USF_LOGIC_OP_EXECUTOR_H
#include "cpe/utils/stream.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_executor_t
logic_executor_basic_create(
    logic_manage_t mgr, 
    const char * name,
    logic_op_fun_t op, void * op_ctx, cfg_t args);

logic_executor_t
logic_executor_group_create(
    logic_manage_t mgr,
    const char * name);

int logic_executor_group_add(logic_executor_t group, logic_executor_t member);

logic_executor_t
logic_executor_decorate_create(
    logic_manage_t mgr,
    const char * name,
    logic_decorate_fun_t op, void * op_ctx, logic_executor_t inner);
int logic_executor_decorate_exec_inner(logic_executor_t executor, logic_context_t ctx);

const char * logic_executor_name(logic_executor_t executor);

void logic_executor_free(logic_executor_t executor);

void logic_executor_dump(logic_executor_t executor, write_stream_t stream, int level);

#ifdef __cplusplus
}
#endif

#endif

