#ifndef INET_SCH_INTERNAL_TYPES_H
#define INET_SCH_INTERNAL_TYPES_H
#include "curl/curl.h"
#include "ev.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "gd/app/app_types.h"
#include "inet/sch/inet_sch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct inet_sch_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    struct ev_loop * m_loop;
    struct ev_timer m_timer_event;
    CURLM * m_multi;
    int m_still_running;
    int m_debug;
};

struct inet_sch_task {
    inet_sch_manage_t m_mgr;
    inet_sch_task_state_t m_state;
    CURL * m_handler;
    struct ev_io m_ev;
    int m_evset;
    char m_error[CURL_ERROR_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif
