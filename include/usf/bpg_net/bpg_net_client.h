#ifndef USF_BPG_NET_CLIENT_TCP_H
#define USF_BPG_NET_CLIENT_TCP_H
#include "bpg_net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_net_client_t
bpg_net_client_create(
    gd_app_context_t app,
    bpg_pkg_manage_t pkg_manage,
    const char * name,
    const char * ip,
    short port,
    mem_allocrator_t alloc,
    error_monitor_t em);

void bpg_net_client_free(bpg_net_client_t svr);

bpg_net_client_t
bpg_net_client_find(gd_app_context_t app, cpe_hash_string_t name);

bpg_net_client_t
bpg_net_client_find_nc(gd_app_context_t app, const char * name);

gd_app_context_t bpg_net_client_app(bpg_net_client_t client);
const char * bpg_net_client_name(bpg_net_client_t client);
cpe_hash_string_t bpg_net_client_name_hs(bpg_net_client_t client);

net_connector_t bpg_net_client_connector(bpg_net_client_t req);
bpg_pkg_manage_t bpg_net_client_pkg_manage(bpg_net_client_t req);

#ifdef __cplusplus
}
#endif

#endif
