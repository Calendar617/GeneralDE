#ifndef GD_NET_ENDPOINT_H
#define GD_NET_ENDPOINT_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*服务管理接口*/
gd_net_ep_t gd_net_ep_find(gd_net_mgr_t nmgr, const char * name);
void gd_net_ep_free(gd_net_ep_t ep);

const char * gd_net_ep_name(gd_net_ep_t ep);
gd_net_chanel_t gd_net_ep_chanel_read(gd_net_ep_t ep);
gd_net_chanel_t gd_net_ep_chanel_write(gd_net_ep_t ep);

/*服务读写接口*/
int ge_net_ep_send(gd_net_ep_t ep, const void * buf, size_t size);
int ge_net_ep_rece(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_peek(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_erase(gd_net_ep_t ep, size_t size);

#ifdef __cplusplus
}
#endif

#endif
