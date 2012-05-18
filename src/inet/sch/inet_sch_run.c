#include <assert.h>
#include "inet/sch/inet_sch_manage.h"
#include "inet_sch_internal_ops.h"

static void inet_sch_timer_cb(EV_P_ struct ev_timer *w, int revents);

int inet_sch_manage_sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void * sockpp) {
    static const char * whatstr[]={ "none", "IN", "OUT", "INOUT", "REMOVE"};
    inet_sch_manage_t mgr;
    inet_sch_task_t task;

    mgr = (inet_sch_manage_t)cbp;
    assert(mgr);

    if (mgr->m_debug >= 2) {
        CPE_INFO(
            mgr->m_em, "%s: socket callback: e %p s %i what %s(%d)",
            inet_sch_manage_name(mgr), e, s,
            (what >= 0 && what < sizeof(whatstr) / sizeof(whatstr[0])) ? whatstr[what] : "???", what);
    }

    curl_easy_getinfo(e, CURLINFO_PRIVATE, &task);
    assert(task);

    if (what == CURL_POLL_REMOVE) {
        inet_sch_task_complete(task);
    }
    else {
        inet_sch_task_set_socket(task, s, what);
    }

    return 0;
}

int inet_sch_manage_timer_update_cb(CURLM *multi, long timeout_ms, inet_sch_manage_t mgr) {
    assert(mgr);

    if (mgr->m_debug >= 2) {
        CPE_INFO(
            mgr->m_em, "%s: timer update: timeout_ms=%ld\n",
            inet_sch_manage_name(mgr), timeout_ms);
    }

    ev_timer_stop(mgr->m_loop, &mgr->m_timer_event);
    if (timeout_ms > 0) {
        double  t = timeout_ms / 1000;
        ev_timer_init(&mgr->m_timer_event, inet_sch_timer_cb, t, 0.);
        ev_timer_start(mgr->m_loop, &mgr->m_timer_event);
    }
    else {
        inet_sch_timer_cb(mgr->m_loop, &mgr->m_timer_event, 0);
    }

    return 0;
}

static void inet_sch_timer_cb(EV_P_ struct ev_timer *w, int revents) {
    CURLMcode rc;
    inet_sch_manage_t mgr = (inet_sch_manage_t)w->data;

    if (mgr->m_debug >= 2) {
        CPE_INFO(
            mgr->m_em, "%s: timer: event=%d",
            inet_sch_manage_name(mgr), revents);
    }

    rc = curl_multi_socket_action(mgr->m_multi, CURL_SOCKET_TIMEOUT, 0, &mgr->m_still_running);
    if (rc != CURLM_OK) {
        CPE_ERROR(
            mgr->m_em, "%s: timer: curl_multi_socket_action fail, %d(%s)",
            inet_sch_manage_name(mgr), rc, inet_sch_curl_code_msg(rc));
    }

    inet_sch_process_multi_info(mgr);
}

void inet_sch_process_multi_info(inet_sch_manage_t mgr) {
    CURLMsg *msg;

    while ((msg = curl_multi_info_read(mgr->m_multi, NULL))) {
        struct inet_sch_task * task;
        CURL * easy;
        CURLcode res;
        char * eff_url;

        easy = msg->easy_handle;
        res = msg->data.result;
        curl_easy_getinfo(easy, CURLINFO_PRIVATE, &task);
        curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);

        switch(msg->msg) {
        case CURLMSG_DONE: {
            if (mgr->m_debug) {
                CPE_INFO(mgr->m_em, "%s: DONE: %s => (%d) %s", inet_sch_manage_name(mgr), eff_url, res, ""/*connection->error*/);
            }
            inet_sch_task_complete(task);
            break;
        }
        default:
            CPE_ERROR(mgr->m_em, "%s: UNKNOWN: %s => (%d) %s", inet_sch_manage_name(mgr), eff_url, res, ""/*connection->error*/);
        }
    }
}
