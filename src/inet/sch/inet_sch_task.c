#include <assert.h>
#include "inet/sch/inet_sch_task.h"
#include "inet/sch/inet_sch_manage.h"
#include "inet_sch_internal_ops.h"

inet_sch_task_t inet_sch_task_create(inet_sch_manage_t mgr) {
    inet_sch_task_t task;

    task = mem_alloc(mgr->m_alloc, sizeof(struct inet_sch_task));
    if (task == NULL) return NULL;

    task->m_mgr = mgr;
    task->m_state = inet_sch_task_state_init;
    task->m_evset = 0;
    task->m_error[0] = '\0';

    task->m_handler = curl_easy_init();
    if (task->m_handler == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: create task: curl_easy_init fail!",
            inet_sch_manage_name(mgr));
        mem_free(mgr->m_alloc, task);
        return NULL;
    }
    curl_easy_setopt(task->m_handler, CURLOPT_PRIVATE, task);
    curl_easy_setopt(task->m_handler, CURLOPT_ERRORBUFFER, task->m_error);

    task->m_ev.data = task;

    return task;
}

void inet_sch_task_free(inet_sch_task_t task) {
    inet_sch_task_complete(task);
    curl_easy_cleanup(task->m_handler);
    mem_free(task->m_mgr->m_alloc, task);
}

int inet_sch_task_run(inet_sch_task_t task) {
    CURLMcode rc;

    assert(task);

    if (task->m_state != inet_sch_task_state_init) {
        CPE_ERROR(
            task->m_mgr->m_em, "%s: run task: task run in error state %d!",
            inet_sch_manage_name(task->m_mgr),
            task->m_state);
        return -1;
    }

    rc = curl_multi_add_handle(task->m_mgr->m_multi, task->m_handler);
    if (rc != CURLM_OK) {
        CPE_ERROR(
            task->m_mgr->m_em, "%s: run task: curl_multi_add_handle fail, %s(%d)!",
            inet_sch_manage_name(task->m_mgr),
            inet_sch_curl_code_msg(rc), rc);
        return -1;
    }

    task->m_state = inet_sch_task_state_runing;

    return 0;
}

void inet_sch_task_complete(inet_sch_task_t task) {
    if (task->m_evset) {
        ev_io_stop(task->m_mgr->m_loop, &task->m_ev);
    }

    if (inet_sch_task_state_runing) {
        curl_multi_remove_handle(task->m_mgr->m_multi, task->m_handler);
    }

    task->m_state = inet_sch_task_state_completed;
}

static void inet_sch_task_sock_event_cb(EV_P_ struct ev_io *w, int revents) {
    inet_sch_manage_t mgr;
    inet_sch_task_t task;
    CURLMcode rc;
    int action;

    task = (inet_sch_task_t)w->data;
    assert(task);

    mgr = task->m_mgr;

    action = (revents & EV_READ?CURL_POLL_IN : 0) | ( revents & EV_WRITE?CURL_POLL_OUT : 0);

    rc = curl_multi_socket_action(task->m_mgr->m_multi, w->fd, action, &mgr->m_still_running);
    if (rc != CURLM_OK) {
        CPE_ERROR(
            mgr->m_em, "%s: sock_event_cb: curl_multi_socket_handle fail, %s(%d)!",
            inet_sch_manage_name(task->m_mgr),
            inet_sch_curl_code_msg(rc), rc);
        return;
    }

    inet_sch_process_multi_info(mgr);
    if (mgr->m_still_running <= 0) {
        if (mgr->m_debug >= 2) {
            CPE_INFO(
                mgr->m_em, "%s: sock_event_cb: no task runing, stop timer!",
                inet_sch_manage_name(task->m_mgr));
        }
        ev_timer_stop(mgr->m_loop, &mgr->m_timer_event);
    }
}

void inet_sch_task_set_socket(inet_sch_task_t task, curl_socket_t s, int act) {
    int kind;
    inet_sch_manage_t mgr;

    kind = (act & CURL_POLL_IN ? EV_READ : 0) | ( act & CURL_POLL_OUT ? EV_WRITE : 0);

    if (task->m_evset) ev_io_stop(mgr->m_loop, &task->m_ev);

    ev_io_init(&task->m_ev, inet_sch_task_sock_event_cb, s, kind);
    ev_io_start(mgr->m_loop, &task->m_ev);

    task->m_evset = 1;
}
