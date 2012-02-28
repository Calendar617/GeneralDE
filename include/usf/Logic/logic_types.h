#ifndef USF_LOGIC_TYPES_H
#define USF_LOGIC_TYPES_H
#include "cpe/pal/pal_types.h"
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t logic_context_id_t;
typedef uint32_t logic_require_id_t;

typedef enum logic_context_state {
    logic_context_idle
    , logic_context_pending
    , logic_context_error
} logic_context_state_t;

typedef enum logic_require_state {
    logic_require_waiting
    , logic_require_error
    , logic_require_done
} logic_require_state_t;

typedef struct logic_manage * logic_manage_t;
typedef struct logic_context * logic_context_t;
typedef struct logic_data * logic_data_t;
typedef struct logic_require * logic_require_t;
typedef struct logic_require_type * logic_require_type_t;
struct logic_op;

#ifdef __cplusplus
}
#endif

#endif
