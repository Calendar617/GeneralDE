#ifndef USF_LOGIC_OP_EXECUTOR_TYPE_H
#define USF_LOGIC_OP_EXECUTOR_TPE_H
#include "cpe/utils/stream.h"
#include "logic_types.h"

#ifdef __cplusplus
extern "C" {
#endif

logic_executor_type_group_t
logic_executor_type_group_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc);

void logic_executor_type_group_free(logic_executor_type_group_t group);

logic_executor_type_group_t
logic_executor_type_group_find(
    gd_app_context_t app,
    cpe_hash_string_t name);

logic_executor_type_group_t
logic_executor_type_group_default(
    gd_app_context_t app);

gd_app_context_t logic_executor_type_group_app(logic_executor_type_group_t group);
const char * logic_executor_type_group_name(logic_executor_type_group_t group);
cpe_hash_string_t logic_executor_type_group_name_hs(logic_executor_type_group_t group);

#ifdef __cplusplus
}
#endif

#endif

