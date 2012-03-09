#ifndef USF_LOGIC_OP_EXECUTOR_H
#define USF_LOGIC_OP_EXECUTOR_H
#include "cpe/utils/stream.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*basic*/
logic_executor_t
logic_executor_basic_create(
    logic_manage_t mgr, 
    const char * name,
    logic_op_fun_t op, void * op_ctx, cfg_t args);

/*group*/
logic_executor_t
logic_executor_group_create(
    logic_manage_t mgr,
    const char * name);

int logic_executor_group_add(logic_executor_t group, logic_executor_t member);

/*decorate*/
logic_executor_t
logic_executor_decorate_create(
    logic_manage_t mgr,
    const char * name,
    logic_decorate_fun_t op, void * op_ctx, logic_executor_t inner);

/*common operations*/
const char * logic_executor_name(logic_executor_t executor);
void logic_executor_free(logic_executor_t executor);
void logic_executor_dump(logic_executor_t executor, write_stream_t stream, int level);

/*protected*/
int32_t logic_executor_decorate_protect(logic_context_t ctx, logic_context_decorate_tag_t tag, void * user_data);
#define logic_executor_protect_create(mgr, name, inner)                  \
    logic_executor_decorate_create(mgr, name, logic_executor_decorate_protect, NULL, inner)

#ifdef __cplusplus
}
#endif

#endif

