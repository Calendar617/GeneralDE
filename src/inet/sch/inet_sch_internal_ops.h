#ifndef INET_SCH_INTERNAL_OPS_H
#define INET_SCH_INTERNAL_OPS_H
#include "inet_sch_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int inet_sch_manage_sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp);
int inet_sch_manage_timer_update_cb(CURLM *multi, long timeout_ms, inet_sch_manage_t mgr);

void inet_sch_task_complete(inet_sch_task_t task);
void inet_sch_task_set_socket(inet_sch_task_t task, curl_socket_t s, int act);

void inet_sch_process_multi_info(inet_sch_manage_t mgr);

const char * inet_sch_curl_code_msg(CURLMcode code);

#ifdef __cplusplus
}
#endif

#endif
