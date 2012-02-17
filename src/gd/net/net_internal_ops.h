#ifndef GD_NET_INTERNAL_OPS_H
#define GD_NET_INTERNAL_OPS_H
#include "net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_ep_t gd_net_ep_pages_alloc_ep(gd_net_mgr_t nmgr);
void gd_net_ep_pages_free_ep(gd_net_ep_t);
void gd_net_ep_pages_free(gd_net_mgr_t nmgr);

/*tcp ops*/
void gd_net_socket_close(int * fd, error_monitor_t em);
int gd_net_socket_set_none_block(int fd, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
