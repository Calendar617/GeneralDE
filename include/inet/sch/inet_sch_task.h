#ifndef INET_SCH_TASK_H
#define INET_SCH_TASK_H
#include "inet_sch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

inet_sch_task_t inet_sch_task_create(inet_sch_manage_t mgr);
void inet_sch_task_free(inet_sch_task_t task);

/*停止运行中的任务*/
void inet_sch_task_cancel(inet_sch_task_t task);
/*启动任务*/
int inet_sch_task_run(inet_sch_task_t task);

/*return CURL handler*/
void * inet_sch_task_handler(inet_sch_task_t task);

#ifdef __cplusplus
}
#endif

#endif

