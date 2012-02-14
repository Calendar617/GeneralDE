#ifndef GD_NET_LISTENER_H
#define GD_NET_LISTENER_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_listener_t
gd_net_listener_create(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port,
    int acceptQueueSize,
    gd_net_accept_fun acceptor);

void gd_net_listener_free(gd_net_listener_t listener);

const char * gd_net_listener_name(gd_net_listener_t listener);

#ifdef __cplusplus
}
#endif

#endif

