#ifndef GD_TL_ACTION_H
#define GD_TL_ACTION_H
#include "cpe/utils/memory.h"
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_tl_event_t gd_tl_event_create(gd_tl_t tl, size_t dataSize);
gd_tl_event_t gd_tl_event_clone(gd_tl_event_t e, mem_allocrator_t alloc);
void gd_tl_event_free(gd_tl_event_t e);

void * gd_tl_event_data(gd_tl_event_t event);
size_t gd_tl_event_capacity(gd_tl_event_t event);
gd_tl_t gd_tl_event_tl(gd_tl_event_t event);
int gd_tl_event_in_queue(gd_tl_event_t event);
gd_tl_event_t gd_tl_event_from_data(void * data);

gd_tl_event_t gd_tl_action_add(gd_tl_t tl);

/* repeatCount: >= 1 execute fix count
                < 0  repeat no limit
 */
int gd_tl_event_send_ex(
    gd_tl_event_t event,
    gd_tl_time_span_t delay,
    gd_tl_time_span_t span,
    int repeatCount);

#ifdef __cplusplus
}
#endif

#endif
