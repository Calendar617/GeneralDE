#include <assert.h>
#include "gd/tl/tl_action.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

gd_tl_event_t gd_tl_event_create(gd_tl_t tl, size_t dataSize) {
    struct gd_tl_event_node * node = gd_tl_event_node_alloc(tl, dataSize);
    if (node == NULL) return NULL;

    TAILQ_INSERT_HEAD(&tl->m_manage->m_event_building_queue, node, m_next);

    return &node->m_event;
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

    if (tl->m_event_enqueue) {
        return tl->m_event_enqueue(
            event, delay, span, repeatCount,
            tl->m_event_op_context);
    }
    else {
        return GD_TL_ERROR_EVENT_NO_ENQUEUE;
    }
}
