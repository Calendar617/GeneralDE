#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/tl/tl_action.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

gd_tl_event_t gd_tl_event_create(gd_tl_t tl, size_t dataSize) {
    struct gd_tl_event_node * node = gd_tl_event_node_alloc(tl, dataSize);
    if (node == NULL) return NULL;

    return &node->m_event;
}

gd_tl_event_t gd_tl_event_clone(gd_tl_event_t e, mem_allocrator_t alloc) {
    struct gd_tl_free_event * r;

    r = (struct gd_tl_free_event *)mem_alloc(alloc, sizeof(struct gd_tl_free_event) + e->m_capacity);
    if (r == NULL) return NULL;

    r->m_alloc = alloc;
    r->m_event.m_tl = NULL;
    r->m_event.m_capacity = e->m_capacity;
    memcpy(r + 1, e + 1, e->m_capacity);

    return &r->m_event;
}

void gd_tl_event_free(gd_tl_event_t e) {
    struct gd_tl_free_event * fe;

    if (e == NULL) return;

    if (e->m_tl == NULL) {
        fe = (struct gd_tl_free_event *)
            (((char*)e)
             - (sizeof(struct gd_tl_free_event) - sizeof(struct gd_tl_event)));
        mem_free(fe->m_alloc, fe);
    }
    else {
        gd_tl_event_node_free(gd_tl_event_to_node(e));
    }
}

gd_tl_event_t gd_tl_action_add(gd_tl_t tl) {
    union gd_tl_action * action;
    int nextEndPos;
    gd_tl_manage_t tm;

    if (tl == NULL || tl->m_manage == NULL) return NULL;

   tm = tl->m_manage;

    nextEndPos = tm->m_action_end_pos + 1;
    if (nextEndPos >= GD_TL_ACTION_MAX) {
        nextEndPos = 0;
    }

    if (nextEndPos == tm->m_action_begin_pos) {
        return NULL;
    }

    action = &tm->m_action_queue[tm->m_action_end_pos];
    tm->m_action_end_pos = nextEndPos;

    action->m_event.m_tl = tl;
    if (tl->m_event_construct) {
        tl->m_event_construct(&action->m_event, tl->m_event_op_context);
    }

    return &action->m_event;
}

void * gd_tl_event_data(gd_tl_event_t event) {
    return (void*)(event + 1);
}

size_t gd_tl_event_capacity(gd_tl_event_t event) {
    return event->m_capacity;
}

gd_tl_t gd_tl_event_tl(gd_tl_event_t event) {
    return event->m_tl;
}

gd_tl_event_t
gd_tl_event_from_data(void * data) {
    return (gd_tl_event_t)( ((char *)data) - sizeof(struct gd_tl_event));
}

int gd_tl_event_enqueue_local(
    gd_tl_event_t event,
    gd_tl_time_span_t delay,
    gd_tl_time_span_t span,
    int repeatCount,
    void * context)
{
    int r;
    struct gd_tl_event_node * input = gd_tl_event_to_node(event);

    input->m_execute_time =
        event->m_tl->m_manage->m_time_current +  delay;
    input->m_span = span;
    input->m_repeatCount = repeatCount;

    /*be careful, input not be managed by manage!!!*/
    gd_tl_event_node_remove_from_building_queue(input);
    input->m_state = gd_tl_event_node_state_free;

    r = gd_tl_event_node_insert(input);
    if (r != 0) {
        gd_tl_event_node_free(input);
        return r;
    }

    return 0;
}

int gd_tl_event_in_queue(gd_tl_event_t event) {
    struct gd_tl_event_node * checkEvent;
    struct gd_tl_event_node * input;
    struct gd_tl_event_node_queue * queue;

    if (event == NULL || event->m_tl == NULL || event->m_tl->m_manage == NULL) {
        return 0;
    }

    queue = &event->m_tl->m_manage->m_event_queue;
    input = gd_tl_event_to_node(event);

    for(checkEvent = TAILQ_FIRST(queue);
        checkEvent != TAILQ_END(queue);
        checkEvent = TAILQ_NEXT(checkEvent, m_next))
    {
        if (checkEvent == input) return 1;
    }

    return 0;
}

int gd_tl_event_in_building_queue(gd_tl_event_t event) {
    struct gd_tl_event_node * checkEvent;
    struct gd_tl_event_node * input;
    struct gd_tl_event_node_queue * queue;

    if (event == NULL || event->m_tl == NULL || event->m_tl->m_manage == NULL) {
        return 0;
    }

    queue = &event->m_tl->m_manage->m_event_building_queue;
    input = gd_tl_event_to_node(event);

    for(checkEvent = TAILQ_FIRST(queue);
        checkEvent != TAILQ_END(queue);
        checkEvent = TAILQ_NEXT(checkEvent, m_next))
    {
        if (checkEvent == input) return 1;
    }

    return 0;
}

int gd_tl_event_send_ex(
    gd_tl_event_t event,
    gd_tl_time_span_t delay,
    gd_tl_time_span_t span,
    int repeatCount)
{
    gd_tl_t tl;
    gd_tl_manage_t tm;
    gd_tl_intercept_t intercept;

    if (event == NULL || event->m_tl == NULL || event->m_tl->m_manage == NULL)
        return GD_TL_ERROR_BAD_ARG;

    tl = event->m_tl;

    tm = tl->m_manage;
    assert(tm);
    if (tm->m_time_cvt) {
        delay = tm->m_time_cvt(delay, tm->m_time_ctx);
        span = tm->m_time_cvt(span, tm->m_time_ctx);
    }

    if (repeatCount == 0) return GD_TL_ERROR_BAD_ARG;
    if (delay < 0) return GD_TL_ERROR_BAD_ARG;
    if (span < 0 || (repeatCount != 1 && span == 0)) return GD_TL_ERROR_BAD_ARG;
    if (!gd_tl_event_in_building_queue(event)) return GD_TL_ERROR_EVENT_UNKNOWN;

    TAILQ_FOREACH(intercept, &tl->m_intercepts, m_next) {
        if (intercept->m_intercept_fun(event, intercept->m_intercept_ctx)) {
            return GD_TL_ERROR_NONE;
        }
    }

    if (tl->m_event_enqueue) {
        return tl->m_event_enqueue(
            event, delay, span, repeatCount,
            tl->m_event_op_context);
    }
    else {
        return GD_TL_ERROR_EVENT_NO_ENQUEUE;
    }
}
