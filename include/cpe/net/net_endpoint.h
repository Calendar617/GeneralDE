#ifndef CPE_NET_ENDPOINT_H
#define CPE_NET_ENDPOINT_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*服务管理接口*/
net_ep_t net_ep_create(net_mgr_t nmgr);
void net_ep_free(net_ep_t ep);

net_ep_t net_ep_find(net_mgr_t nmgr, net_ep_id_t id);

net_ep_id_t net_ep_id(net_ep_t ep);
net_chanel_t net_ep_chanel_r(net_ep_t ep);
net_chanel_t net_ep_chanel_w(net_ep_t ep);
void net_ep_set_chanel_r(net_ep_t ep, net_chanel_t chanel);
void net_ep_set_chanel_w(net_ep_t ep, net_chanel_t chanel);

void net_ep_close(net_ep_t ep);

int net_ep_is_open(net_ep_t ep);
net_connector_t net_ep_connector(net_ep_t ep);

void net_ep_set_processor(net_ep_t ep, net_process_fun_t process_fun, void * process_ctx);

/*服务读写接口*/
int net_ep_send(net_ep_t ep, const void * buf, size_t size);
ssize_t net_ep_rece(net_ep_t ep, void * buf, size_t capacity);

char * net_ep_peek(net_ep_t ep, void * buf, size_t size);
void net_ep_erase(net_ep_t ep, size_t size);

#ifdef __cplusplus
}
#endif

#endif
