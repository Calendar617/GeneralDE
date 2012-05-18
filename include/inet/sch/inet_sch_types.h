#ifndef INET_SCH_TYPES_H
#define INET_SCH_TYPES_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct inet_sch_task * inet_sch_task_t;
typedef struct inet_sch_manage * inet_sch_manage_t;

typedef enum inet_sch_task_state {
    inet_sch_task_state_init
    , inet_sch_task_state_runing
    , inet_sch_task_state_completed
} inet_sch_task_state_t;

#ifdef __cplusplus
}
#endif

#endif
