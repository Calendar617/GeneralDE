#ifndef CPE_DP_TIMER_TYPES_H
#define CPE_DP_TIMER_TYPES_H
#include "cpe/utils/hash_string.h"
#include "cpe/tl/tl_types.h"
#include "gd/app/app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t gd_timer_id_t;

typedef struct gd_timer_mgr * gd_timer_mgr_t;
typedef struct gd_evt_processor * gd_evt_responser_t;

typedef void (*gd_timer_process_fun_t)(void * ctx, gd_timer_id_t timer_id, void * arg);

#ifdef __cplusplus
}
#endif

#endif
