#ifndef GD_NET_MANAGE_H
#define GD_NET_MANAGE_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_mgr_t gd_net_mgr_create(mem_allocrator_t alloc);
void gd_net_mgr_free(gd_net_mgr_t);

void gd_net_mgr_run(gd_net_mgr_t nmgr);

#ifdef __cplusplus
}
#endif

#endif
