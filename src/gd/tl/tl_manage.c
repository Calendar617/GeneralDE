#include <stdarg.h>
#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "gd/tl/tl_intercept.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc) {
    int i;

    gd_tl_manage_t tm = (gd_tl_manage_t)mem_alloc(alloc, sizeof(struct gd_tl_manage));
    if (tm == NULL) return NULL;

    tm->m_alloc = alloc;
    
    tm->m_time_get = gd_tl_time_source_msec;
    tm->m_time_cvt = 0;
    tm->m_time_ctx = 0;
    tm->m_time_current = tm->m_time_get(tm->m_time_ctx);

    tm->m_action_begin_pos = tm->m_action_end_pos = 0;

    for(i = 0; i < GD_TL_ACTION_MAX; ++i) {
        union gd_tl_action * action = &tm->m_action_queue[i];
        action->m_event.m_tl = NULL;
        action->m_event.m_capacity = GD_TL_ACTION_MAX - sizeof(struct gd_tl_event);
    }
 
    TAILQ_INIT(&tm->m_tls);

    TAILQ_INIT(&tm->m_event_queue);
    TAILQ_INIT(&tm->m_event_building_queue);

    return tm;
}

static void gd_tl_action_queue_clear(gd_tl_manage_t tm) {
    if (tm->m_action_begin_pos > tm->m_action_end_pos) {
        for(; tm->m_action_begin_pos < GD_TL_ACTION_MAX;
            ++tm->m_action_begin_pos)
        {
            union gd_tl_action * action = &tm->m_action_queue[tm->m_action_begin_pos];
            if (action->m_event.m_tl->m_event_destory) {
                action->m_event.m_tl->m_event_destory(
                    &action->m_event,
                    action->m_event.m_tl->m_event_op_context);
            }
        }

        assert(tm->m_action_begin_pos >= GD_TL_ACTION_MAX);
        tm->m_action_begin_pos = 0;
    }

    for(; tm->m_action_begin_pos < tm->m_action_end_pos; ++tm->m_action_begin_pos) {
        union gd_tl_action * action = &tm->m_action_queue[tm->m_action_begin_pos];
        if (action->m_event.m_tl->m_event_destory) {
            action->m_event.m_tl->m_event_destory(
                &action->m_event,
                action->m_event.m_tl->m_event_op_context);
        }
    }
}

void gd_tl_queue_clear(gd_tl_manage_t tm) {
    while(!TAILQ_EMPTY(&tm->m_tls)) {
        gd_tl_free(TAILQ_FIRST(&tm->m_tls));
    }
}

void gd_tl_manage_free(gd_tl_manage_t tm) {
    if (tm == NULL) return;

    gd_tl_event_queue_clear(&tm->m_event_building_queue);
    gd_tl_event_queue_clear(&tm->m_event_queue);
    gd_tl_action_queue_clear(tm);
    gd_tl_queue_clear(tm);

    mem_free(tm->m_alloc, tm);
}

int gd_tl_manage_set_opt(gd_tl_manage_t tm, gd_tl_manage_option_t opt,...) {
    int rv = -1;
    va_list ap;
    va_start(ap, opt);

    switch(opt) {
        case gd_tl_set_time_source: {
            gd_tl_time_fun_t ts = va_arg(ap, gd_tl_time_fun_t);
            if (ts == NULL) {
                rv = GD_TL_ERROR_NO_TIME_SOURCE;
            }
            else {
                rv = 0;
                tm->m_time_get = ts;
                tm->m_time_current = ts(tm->m_time_ctx);
            }
            break;
        }
        case gd_tl_set_time_cvt: {
            rv = 0;
            tm->m_time_cvt = va_arg(ap, gd_tl_time_cvt_fun_t);
            break;
        }
        case gd_tl_set_time_source_context: {
            rv = 0;
            tm->m_time_ctx = va_arg(ap, void *);
            break;
        }
        default:
            rv = GD_TL_ERROR_BAD_ARG;
            break;
    }

    va_end(ap);

    return rv;
}

gd_tl_t gd_tl_create(gd_tl_manage_t tm) {
    gd_tl_t tl;

    tl = mem_alloc(tm->m_alloc, sizeof(struct gd_tl));
    if (tl == NULL) return NULL;

    tl->m_manage = tm;
    tl->m_event_dispatcher = NULL;
    tl->m_event_enqueue = gd_tl_event_enqueue_local;
    tl->m_event_construct = NULL;
    tl->m_event_destory = NULL;
    tl->m_event_op_context = NULL;
    TAILQ_INIT(&tl->m_events);
    TAILQ_INIT(&tl->m_intercepts);

    TAILQ_INSERT_TAIL(&tm->m_tls, tl, m_next);

    return tl;
}

void gd_tl_free(gd_tl_t tl) {
    if (tl == NULL) return;

    while(!TAILQ_EMPTY(&tl->m_events)) {
        gd_tl_event_node_free(TAILQ_FIRST(&tl->m_events));
    }

    while(!TAILQ_EMPTY(&tl->m_intercepts)) {
        gd_tl_intercept_free(TAILQ_FIRST(&tl->m_intercepts));
    }

    TAILQ_REMOVE(&tl->m_manage->m_tls, tl, m_next);

    mem_free(tl->m_manage->m_alloc, tl);
}

gd_tl_time_t gd_tl_manage_time(gd_tl_manage_t tm) {
    return tm->m_time_current;
}

int gd_tl_set_opt(gd_tl_t tl, gd_tl_option_t opt, ...) {
    int rv = -1;
    va_list ap;
    va_start(ap, opt);

    switch(opt) {
        case gd_tl_set_event_dispatcher: {
            rv = 0;
            tl->m_event_dispatcher = va_arg(ap, gd_tl_event_process_t);
            break;
        }
        case gd_tl_set_event_enqueue: {
            gd_tl_event_enqueue_t enqueue = va_arg(ap, gd_tl_event_enqueue_t);
            if (enqueue == NULL) {
                rv = GD_TL_ERROR_EVENT_NO_ENQUEUE;
            }
            else {
                rv = 0;
                tl->m_event_enqueue = enqueue;
            }
            break;
        }
        case gd_tl_set_event_construct: {
            rv = 0;
            tl->m_event_construct = va_arg(ap, gd_tl_event_process_t);
            break;
        }
        case gd_tl_set_event_destory: {
            rv = 0;
            tl->m_event_destory = va_arg(ap, gd_tl_event_process_t);
            break;
        }
        case gd_tl_set_event_op_context: {
            rv = 0;
            tl->m_event_op_context = va_arg(ap, void *);
            break;
        }
        default:
            rv = GD_TL_ERROR_BAD_ARG;
            break;
    }

    va_end(ap);

    return rv;
}
