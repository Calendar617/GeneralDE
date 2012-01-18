#include <limits.h>
#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

#define gd_tl_event_do_dispatch(evt, rv)                                \
    if ((evt)->m_tl->m_event_dispatcher) {                              \
        (evt)->m_tl->m_event_dispatcher(                                \
            (evt), (evt)->m_tl->m_event_op_context);                    \
    }                                                                   \
    else {                                                              \
        rv = GD_TL_ERROR_EVENT_NO_DISPATCHER;                           \
    }

#define gd_tl_action_do_dispatch(action, rv)                            \
do {                                                                    \
    union gd_tl_action * __action = action;                             \
    gd_tl_t tl = __action->m_event.m_tl;                                \
    assert(tl);                                                         \
    assert(tl->m_manage);                                               \
    gd_tl_event_do_dispatch(&__action->m_event, rv);                    \
    if (tl->m_event_destory) {                                          \
        tl->m_event_destory(&__action->m_event, tl->m_event_op_context); \
    }                                                                   \
    __action->m_event.m_tl = NULL;                                      \
} while(0)

#define gd_tl_manage_update_time(tm) do { \
    gd_tl_time_t nextTime =  (tm)->m_time_get((tm)->m_time_ctx);    \
    if (nextTime > (tm)->m_time_current) \
        (tm)->m_time_current = nextTime; \
    } while(0)

static int gd_tl_manage_dispatch_action(gd_tl_manage_t tm, int maxCount) {
    int count = 0;
    int rv = 0;

    if (tm->m_action_begin_pos > tm->m_action_end_pos) {
        for(; rv == 0 && maxCount > 0 && tm->m_action_begin_pos < GD_TL_ACTION_MAX;
            ++tm->m_action_begin_pos, ++count, --maxCount)
        {
            gd_tl_action_do_dispatch(&tm->m_action_queue[tm->m_action_begin_pos], rv);
            gd_tl_event_queue_clear(&tm->m_event_building_queue);
        }

        if (tm->m_action_begin_pos >= GD_TL_ACTION_MAX) {
            tm->m_action_begin_pos = 0;
        }
    }

    assert(tm->m_action_begin_pos <= tm->m_action_end_pos);

    for(; rv == 0 && maxCount > 0 && tm->m_action_begin_pos < tm->m_action_end_pos;
            ++tm->m_action_begin_pos, ++count, --maxCount)
    {
        gd_tl_action_do_dispatch(&tm->m_action_queue[tm->m_action_begin_pos], rv);
        gd_tl_event_queue_clear(&tm->m_event_building_queue);
    }

    return rv == 0 ? count : rv;
}

static int gd_tl_manage_dispatch_event(gd_tl_manage_t tm, int maxCount) {
    int rv = 0;
    int count = 0;

    for(; rv == 0
            && maxCount > 0
            && tm->m_action_begin_pos == tm->m_action_end_pos;
        --maxCount)
    {
        struct gd_tl_event_node * node = TAILQ_FIRST(&tm->m_event_queue);

        if (node == TAILQ_END(tm->m_event_queue)
            || node->m_execute_time > tm->m_time_current)
        {
            break;
        }

        TAILQ_REMOVE(&tm->m_event_queue, node, m_next);
        node->m_state = gd_tl_event_node_state_free;

        gd_tl_event_do_dispatch(&node->m_event, rv);
        gd_tl_event_queue_clear(&tm->m_event_building_queue);

        gd_tl_event_node_free(node);

        ++count;
    }

    return rv == 0 ? count : rv;
}

int gd_tl_manage_tick(gd_tl_manage_t tm, int count) {
    int leftCount;

    if (tm == NULL || count == 0) return GD_TL_ERROR_BAD_ARG;
    if (tm->m_time_get == NULL) return GD_TL_ERROR_NO_TIME_SOURCE;

    if (count < 0) count = INT_MAX;

    gd_tl_manage_update_time(tm);

    leftCount = count;
    while(leftCount > 0) {
        int actionDispatchedCount;
        int eventDispatchedCount;

        actionDispatchedCount = gd_tl_manage_dispatch_action(tm, leftCount);
        if (actionDispatchedCount < 0) return actionDispatchedCount; /*have error*/
        assert(actionDispatchedCount <= leftCount);
        leftCount -= actionDispatchedCount;

        eventDispatchedCount = gd_tl_manage_dispatch_event(tm, leftCount);
        if (eventDispatchedCount < 0) return eventDispatchedCount; /*have error*/
        assert(eventDispatchedCount <= leftCount);
        leftCount -= eventDispatchedCount;

        if (actionDispatchedCount + eventDispatchedCount == 0) {
            break;
        }
    }

    if (leftCount == count) {
        /*if no event processed, clear building queue also*/
        gd_tl_event_queue_clear(&tm->m_event_building_queue);
    }

    return (count - leftCount);
}
