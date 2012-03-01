#ifndef CPE_NET_CONNECTOR_H
#define CPE_NET_CONNECTOR_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

net_connector_t
net_connector_create(
    net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port);

net_connector_t
net_connector_create_with_ep(
    net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port);

void net_connector_free(net_connector_t connector);

net_connector_t
net_connector_find(net_mgr_t nmgr, const char * name);

const char * net_connector_name(net_connector_t connector);
net_connector_state_t net_connector_state(net_connector_t connector);

int net_connector_bind(net_connector_t connector, net_ep_t ep);
int net_connector_unbind(net_connector_t connector);

int net_connector_enable(net_connector_t connector);
void net_connector_disable(net_connector_t connector);

void net_connector_set_monitor(
    net_connector_t connector,
    net_connector_state_monitor_fun_t fun, void * ctx);

#ifdef __cplusplus
}
#endif

#endif

