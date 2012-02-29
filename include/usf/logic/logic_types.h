#ifndef USF_LOGIC_TYPES_H
#define USF_LOGIC_TYPES_H
#include "cpe/cfg/cfg_types.h"
#include "gd/app/app_types.h"
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_LOGIC_CONTEXT_ID ((logic_require_id_t)-1)

typedef uint32_t logic_context_id_t;
typedef uint32_t logic_require_id_t;

typedef enum logic_context_state {
    logic_context_idle
    , logic_context_waiting
    , logic_context_error
} logic_context_state_t;

typedef enum logic_require_state {
    logic_require_waiting
    , logic_require_error
    , logic_require_done
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

typedef int32_t (*logic_op_fun_t) (gd_app_context_t app, logic_context_t ctx, void * user_data, cfg_t cfg);
typedef int32_t (*logic_decorate_fun_t) (gd_app_context_t app, logic_context_t ctx, logic_executor_t self, void * user_data);

#ifdef __cplusplus
}
#endif

#endif
