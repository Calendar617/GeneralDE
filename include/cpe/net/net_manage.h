#ifndef CPE_NET_MANAGE_H
#define CPE_NET_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

net_mgr_t net_mgr_create(mem_allocrator_t alloc, error_monitor_t em);
void net_mgr_free(net_mgr_t);

error_monitor_t net_mgr_em(net_mgr_t nmgr);
void net_mgr_break(net_mgr_t nmgr);
void net_mgr_stop(net_mgr_t nmgr);
int net_mgr_tick(net_mgr_t nmgr);
int net_mgr_run(net_mgr_t nmgr);

int net_mgr_debug(net_mgr_t nmgr);
void net_mgr_set_debug(net_mgr_t nmgr, int debug);

#ifdef __cplusplus
}
#endif

#endif
