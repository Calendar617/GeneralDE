#ifndef GD_NET_CONNECTOR_H
#define GD_NET_CONNECTOR_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_connector_t
gd_net_connector_create(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port);

gd_net_connector_t
gd_net_connector_create_with_ep(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port);

void gd_net_connector_free(gd_net_connector_t connector);

gd_net_connector_t
gd_net_connector_find(gd_net_mgr_t nmgr, const char * name);

const char * gd_net_connector_name(gd_net_connector_t connector);
gd_net_connector_state_t gd_net_connector_state(gd_net_connector_t connector);

int gd_net_connector_bind(gd_net_connector_t connector, gd_net_ep_t ep);
int gd_net_connector_unbind(gd_net_connector_t connector);

int gd_net_connector_enable(gd_net_connector_t connector);
int gd_net_connector_disable(gd_net_connector_t connector);

#ifdef __cplusplus
}
#endif

#endif

