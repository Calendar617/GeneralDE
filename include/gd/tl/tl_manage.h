#ifndef GD_TL_MANAGE_H
#define GD_TL_MANAGE_H
#include "cpe/utils/memory.h"
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gd_tl_manage_option {
    gd_tl_set_time_source
    , gd_tl_set_time_source_context
} gd_tl_manage_option_t;

typedef gd_tl_time_t (*gd_tl_time_fun_t)(void * context);

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc);
void gd_tl_manage_free(gd_tl_manage_t tm);
void gd_tl_manage_tick(gd_tl_manage_t tm);
int gd_tl_manage_set_opt(gd_tl_manage_t tm, gd_tl_manage_option_t opt,...);
gd_tl_time_t gd_tl_manage_time(gd_tl_manage_t tm);

typedef int(*gd_tl_event_dispatcher_t)(gd_tl_event_t event, void * context);

gd_tl_t gd_tl_create(gd_tl_manage_t tm, gd_tl_event_dispatcher_t dispatch, void * context);

#ifdef __cplusplus
}
#endif

#endif
