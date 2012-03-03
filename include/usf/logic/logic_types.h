#ifndef USF_LOGIC_TYPES_H
#define USF_LOGIC_TYPES_H
#include "cpe/utils/error.h"
#include "cpe/cfg/cfg_types.h"
#include "gd/app/app_types.h"
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_LOGIC_CONTEXT_ID ((logic_require_id_t)-1)

typedef uint32_t logic_context_id_t;
typedef uint32_t logic_require_id_t;

typedef enum logic_context_flag {
    logic_context_flag_debug = 1 << 0
    , logic_context_flag_execute_immediately = 1 << 1
    , logic_context_flag_require_keep = 1 << 2
} logic_context_flag_t;

typedef enum logic_context_state {
    logic_context_state_init
    , logic_context_state_idle
    , logic_context_state_waiting
    , logic_context_state_error
    , logic_context_state_done
    , logic_context_state_timeout
} logic_context_state_t;

typedef enum logic_require_state {
    logic_require_state_waiting
    , logic_require_state_error
    , logic_require_state_done
} logic_require_state_t;

typedef enum logic_executor_type {
    logic_executor_type_basic
    , logic_executor_type_group
    , logic_executor_type_decorate
} logic_executor_type_t;

typedef struct logic_manage * logic_manage_t;
typedef struct logic_context * logic_context_t;
typedef struct logic_data * logic_data_t;
typedef struct logic_require * logic_require_t;
typedef struct logic_require_type * logic_require_type_t;

typedef struct logic_executor * logic_executor_t;

typedef int32_t (*logic_op_fun_t) (logic_context_t ctx, void * user_data, cfg_t cfg);

typedef enum logic_context_decorate_tag {
    logic_context_decorate_begin,
    logic_context_decorate_end
} logic_context_decorate_tag_t;
typedef int32_t (*logic_decorate_fun_t) (logic_context_t ctx, logic_context_decorate_tag_t tag, void * user_data);

typedef logic_executor_t (*logic_executor_build_fun_t) (
    logic_manage_t mgr, const char * name, void * ctx,
    cfg_t args,
    error_monitor_t em);

typedef void (*logic_context_commit_fun_t) (logic_context_t ctx, void * user_data);

#ifdef __cplusplus
}
#endif

#endif
