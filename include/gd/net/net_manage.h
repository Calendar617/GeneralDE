#ifndef GD_NET_MANAGE_H
#define GD_NET_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_mgr_t gd_net_mgr_create(mem_allocrator_t alloc, error_monitor_t em);
void gd_net_mgr_free(gd_net_mgr_t);

error_monitor_t gd_net_mgr_em(gd_net_mgr_t nmgr);

int gd_net_mgr_tick(gd_net_mgr_t nmgr);

#ifdef __cplusplus
}
#endif

#endif
