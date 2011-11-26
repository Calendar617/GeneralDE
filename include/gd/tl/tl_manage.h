#ifndef GD_TL_MANAGE_H
#define GD_TL_MANAGE_H
#include "cpe/utils/memory.h"
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*time line manager*/
typedef enum gd_tl_manage_option {
    gd_tl_set_time_source
    , gd_tl_set_time_cvt
    , gd_tl_set_time_source_context
} gd_tl_manage_option_t;

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc);
void gd_tl_manage_free(gd_tl_manage_t tm);
int gd_tl_manage_tick(gd_tl_manage_t tm);
int gd_tl_manage_set_opt(gd_tl_manage_t tm, gd_tl_manage_option_t opt,...);
gd_tl_time_t gd_tl_manage_time(gd_tl_manage_t tm);

/*time line*/
typedef enum gd_tl_option {
    gd_tl_set_event_dispatcher
    , gd_tl_set_event_enqueue
    , gd_tl_set_event_destory
    , gd_tl_set_event_op_context
} gd_tl_option_t;

gd_tl_t gd_tl_create(gd_tl_manage_t tm);
int gd_tl_set_opt(gd_tl_t tl, gd_tl_option_t opt, ...);

/*event enqueue*/
int gd_tl_event_enqueue_local(
    gd_tl_event_t event,
    gd_tl_time_span_t delay,
    gd_tl_time_span_t span,
    int repeatCount,
    void * context);

/*time source*/
gd_tl_time_t gd_tl_time_source_msec(void * context);
gd_tl_time_t gd_tl_time_source_usec(void * context);
gd_tl_time_t gd_tl_time_source_next_event(void * context);
gd_tl_time_t gd_tl_time_source_last_event(void * context);

/*time convert*/
gd_tl_time_span_t gd_tl_time_cvt_sec2msec(gd_tl_time_span_t sec, void *);
gd_tl_time_span_t gd_tl_time_cvt_sec2usec(gd_tl_time_span_t sec, void *);

#ifdef __cplusplus
}
#endif

#endif
