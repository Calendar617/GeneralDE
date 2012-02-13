#ifndef GD_NET_INTERNAL_OPS_H
#define GD_NET_INTERNAL_OPS_H
#include "net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_ep_t
gd_net_ep_crate_i(
    gd_net_mgr_t nmgr, 
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    size_t total_capacity);

void gd_net_ep_free_i(gd_net_ep_t ep);

/*tcp ops*/
int gd_net_socket_open(int * fd, error_monitor_t em);
void gd_net_socket_close(int * fd, error_monitor_t em);
int gd_net_socket_set_none_block(int fd, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
