#ifndef GD_NET_INTERNAL_OPS_H
#define GD_NET_INTERNAL_OPS_H
#include "net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*connector op*/
uint32_t gd_net_connector_hash(gd_net_connector_t connector);
int gd_net_connector_cmp(gd_net_connector_t l, gd_net_connector_t r);
void gd_net_connectors_free(gd_net_mgr_t nmgr);

/*listener op*/
uint32_t gd_net_listener_hash(gd_net_listener_t listener);
int gd_net_listener_cmp(gd_net_listener_t l, gd_net_listener_t r);
void gd_net_listeners_free(gd_net_mgr_t nmgr);

/*ep_pages op*/
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
