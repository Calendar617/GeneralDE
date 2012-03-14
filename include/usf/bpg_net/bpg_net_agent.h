#ifndef USF_BPG_NET_AGENT_TCP_H
#define USF_BPG_NET_AGENT_TCP_H
#include "bpg_net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_net_agent_t bpg_net_agent_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em);

void bpg_net_agent_free(bpg_net_agent_t svr);

bpg_net_agent_t
bpg_net_agent_find(gd_app_context_t app, cpe_hash_string_t name);

bpg_net_agent_t
bpg_net_agent_find_nc(gd_app_context_t app, const char * name);

bpg_net_agent_t
bpg_net_agent_default(gd_app_context_t app);

gd_app_context_t bpg_net_agent_app(bpg_net_agent_t mgr);
const char * bpg_net_agent_name(bpg_net_agent_t mgr);
cpe_hash_string_t bpg_net_agent_name_hs(bpg_net_agent_t mgr);

#ifdef __cplusplus
}
#endif

#endif
