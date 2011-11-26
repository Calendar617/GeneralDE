#include <sys/time.h>
#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_ops.h"

gd_tl_time_t gd_tl_time_source_usec(void * context) {
    struct timeval tp;
    int r;
    r = gettimeofday(&tp, NULL);
    assert(r);
    return tp.tv_usec;
}

gd_tl_time_t gd_tl_time_source_msec(void * context) {
    struct timeval tp;
    int r;
    r = gettimeofday(&tp, NULL);
    assert(r);
    return tp.tv_usec / 1000;
}

gd_tl_time_t gd_tl_time_source_next_event(void * context) {
    gd_tl_manage_t tm = (gd_tl_manage_t)context;
    if (TAILQ_EMPTY(&tm->m_event_queue)) {
        return tm->m_time_current;
    }
    else {
        return TAILQ_FIRST(&tm->m_event_queue)->m_execute_time;
    }
}

gd_tl_time_span_t gd_tl_time_cvt_sec2msec(gd_tl_time_span_t sec, void * context) {
    return sec * 1000;
}

gd_tl_time_span_t gd_tl_time_cvt_sec2usec(gd_tl_time_span_t sec, void * context) {
    return sec * 1000 * 1000;
}
