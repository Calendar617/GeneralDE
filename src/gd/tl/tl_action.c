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
    struct gd_tl_event_node * checkEvent;
    struct gd_tl_event_node * input;
    struct gd_tl_event_node_queue * inputQueue;
    int r;

    if (event == NULL || delay < 0 || span < 0) return -1;

    inputQueue = &event->m_tl->m_manage->m_event_building_queue;
    input = (struct gd_tl_event_node *)
        ((char *)event
         - (sizeof(struct gd_tl_event_node)
            - sizeof(struct gd_tl_event)));

    for(checkEvent = TAILQ_FIRST(inputQueue);
        checkEvent != input && checkEvent != TAILQ_END(inputQueue);
        checkEvent = TAILQ_NEXT(checkEvent, m_next))
    {
    }

    if (checkEvent != input) return GD_TL_ERROR_EVENT_UNKNOWN;

    input->m_execute_time =
        event->m_tl->m_manage->m_time_current +  delay;
    input->m_span = span;
    input->m_repeatCount = repeatCount;

    r = gd_tl_event_node_insert(input);
    if (r != 0) return r;

    TAILQ_REMOVE(inputQueue, input, m_next);

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

    if (event == NULL) return -1;

    tl = event->m_tl;
    assert(tl);

    tm = tl->m_manage;
    assert(tm);
    if (tm->m_time_cvt) {
        delay = tm->m_time_cvt(delay, tm->m_time_ctx);
        span = tm->m_time_cvt(span, tm->m_time_ctx);
    }

    assert(tl->m_event_enqueue);
    return tl->m_event_enqueue(event, delay, span, repeatCount, tl->m_event_op_context);
}
