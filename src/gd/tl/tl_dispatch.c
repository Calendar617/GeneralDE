#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

#define gd_tl_event_do_dispatch(evt) \
    (evt)->m_tl->m_dispatcher((evt), (evt)->m_tl->m_dispatcher_context)

#define gd_tl_manage_update_time(tm) \
    (tm)->m_time_current = (tm)->m_time_get((tm)->m_time_ctx)

static int gd_tl_manage_dispatch_action(gd_tl_manage_t tm, int maxCount) {
    int count = 0;

    if (tm->m_action_begin_pos > tm->m_action_end_pos) {
        for(; maxCount > 0 && tm->m_action_begin_pos < GD_TL_ACTION_MAX;
            ++tm->m_action_begin_pos, ++count, --maxCount)
        {
            gd_tl_event_t evt = &tm->m_action_queue[tm->m_action_begin_pos];
            gd_tl_event_do_dispatch(evt);
        }

        if (tm->m_action_begin_pos >= GD_TL_ACTION_MAX) {
            tm->m_action_begin_pos = 0;
        }

        if (maxCount <= 0) {
            return count;
        }
    }

    assert(tm->m_action_begin_pos <= tm->m_action_end_pos);

    for(; maxCount > 0 && tm->m_action_begin_pos < tm->m_action_end_pos;
            ++tm->m_action_begin_pos, ++count, --maxCount)
    {
        gd_tl_event_t evt = &tm->m_action_queue[tm->m_action_begin_pos];
        gd_tl_event_do_dispatch(evt);
    }

    return count;
}

static int gd_tl_manage_dispatch_event(gd_tl_manage_t tm, int maxCount) {
    int count = 0;

    for(; maxCount > 0; --maxCount) {
        struct gd_tl_event_node * nextEvent = TAILQ_FIRST(&tm->m_event_queue);

        if (nextEvent == TAILQ_END(tm->m_event_queue)
            || nextEvent->m_execute_time > tm->m_time_current)
        {
            break;
        }

        TAILQ_REMOVE(&tm->m_event_queue, nextEvent, m_next);

        gd_tl_event_do_dispatch(&nextEvent->m_event);

        gd_tl_event_node_free(nextEvent);
    }

    return count;
}

int gd_tl_manage_tick(gd_tl_manage_t tm) {
    int totalDispatchedCount = 0;
    int partDispatchedCount = 0;

    if (tm == NULL 
        || tm->m_time_get == NULL)
        return -1;

    gd_tl_manage_update_time(tm);

    do {
        if (partDispatchedCount >= GD_TL_TIME_UPDATE_DISPATCH_COUNT) {
            gd_tl_manage_update_time(tm);
            totalDispatchedCount += partDispatchedCount;
            partDispatchedCount = 0;
        }

        partDispatchedCount += gd_tl_manage_dispatch_action(tm, GD_TL_TIME_UPDATE_DISPATCH_COUNT);
        assert(partDispatchedCount <= GD_TL_TIME_UPDATE_DISPATCH_COUNT);

        partDispatchedCount += gd_tl_manage_dispatch_event(tm, GD_TL_TIME_UPDATE_DISPATCH_COUNT - partDispatchedCount);
        assert(partDispatchedCount <= GD_TL_TIME_UPDATE_DISPATCH_COUNT);
    } while(partDispatchedCount > 0);

    return totalDispatchedCount;
}
