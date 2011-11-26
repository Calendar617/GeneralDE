#include <stdarg.h>
#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"
#include "tl_internal_ops.h"

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc) {
    gd_tl_manage_t tm = mem_alloc(alloc, sizeof(gd_tl_manage_t));
    if (tm == NULL) return NULL;

    tm->m_alloc = alloc;
    
    tm->m_time_get = gd_tl_time_source_msec;
    tm->m_time_cvt = gd_tl_time_cvt_sec2msec;
    tm->m_time_ctx = 0;
    tm->m_time_current = tm->m_time_get(tm->m_time_ctx);

    tm->m_tl_count = 0;

    tm->m_action_begin_pos = tm->m_action_end_pos = 0;

    TAILQ_INIT(&tm->m_event_queue);
    TAILQ_INIT(&tm->m_event_building_queue);

    return tm;
}

void gd_tl_manage_free(gd_tl_manage_t tm) {
    if (tm == NULL) return;

    gd_tl_event_queue_clear(&tm->m_event_building_queue);
    gd_tl_event_queue_clear(&tm->m_event_queue);

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
            }
            break;
        }
        case gd_tl_set_time_source_context: {
            rv = 0;
            tm->m_time_ctx = va_arg(ap, void *);
            break;
        }
    }

    va_end(ap);

    return rv;
}

gd_tl_t gd_tl_create(gd_tl_manage_t tm, gd_tl_event_dispatcher_t dispatch, void * context) {
    gd_tl_t tl;

    if (dispatch == NULL) return NULL;
    if (tm->m_tl_count >= GD_TL_TL_MAX) return NULL;

    tl = &tm->m_tls[tm->m_tl_count++];

    tl->m_manage = tm;
    tl->m_event_dispatcher = dispatch;
    tl->m_event_op_context = context;
    tl->m_event_destory = NULL;
    
    return tl;
}

int gd_tl_set_opt(gd_tl_t tl, gd_tl_option_t opt, ...) {
    int rv = -1;
    va_list ap;
    va_start(ap, opt);

    switch(opt) {
        case gd_tl_set_event_dispatcher: {
            gd_tl_event_dispatcher_t dispatch = va_arg(ap, gd_tl_event_dispatcher_t);
            if (dispatch == NULL) {
                rv = GD_TL_ERROR_EVENT_NO_DISPATCHER;
            }
            else {
                rv = 0;
                tl->m_event_dispatcher = dispatch;
            }
            break;
        }
        case gd_tl_set_event_destory: {
            rv = 0;
            tl->m_event_destory = va_arg(ap, gd_tl_event_destory_t);
            break;
        }
        case gd_tl_set_event_op_context: {
            rv = 0;
            tl->m_event_op_context = va_arg(ap, void *);
            break;
        }
    }

    va_end(ap);

    return rv;
}
