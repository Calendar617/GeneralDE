#ifndef USF_BPG_NET_INTERNAL_OPS_H
#define USF_BPG_NET_INTERNAL_OPS_H
#include "cpe/net/net_types.h"
#include "bpg_net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*agent process*/
void bpg_net_agent_accept(net_listener_t listener, net_ep_t ep, void * ctx);
int bpg_net_agent_reply(dp_req_t req, void * ctx, error_monitor_t em);
bpg_pkg_t bpg_net_agent_req_buf(bpg_net_agent_t mgr);

/*binding operations*/
int bpg_net_agent_binding_create(
    bpg_net_agent_t mgr,
    uint64_t client_id,
    uint32_t connection_id);

void bpg_net_agent_binding_free(bpg_net_agent_t mgr, struct bpg_net_agent_binding * binding);

struct bpg_net_agent_binding *
bpg_net_agent_binding_find_by_client_id(bpg_net_agent_t mgr, uint64_t client_id);

struct bpg_net_agent_binding *
bpg_net_agent_binding_find_by_connection_id(bpg_net_agent_t mgr, uint32_t connection_id);

net_ep_t bpg_net_agent_process_reply(bpg_net_agent_t mgr, uint64_t client_id, uint32_t connection_id);
bpg_net_pkg_next_step_t bpg_net_agent_process_recv(bpg_net_agent_t mgr, uint64_t client_id, uint32_t connection_id);

uint32_t bpg_net_agent_binding_client_id_hash(
    const struct bpg_net_agent_binding * binding);
int bpg_net_agent_binding_client_id_cmp(
    const struct bpg_net_agent_binding * l,
    const struct bpg_net_agent_binding * r);
uint32_t bpg_net_agent_binding_connection_id_hash(
    const struct bpg_net_agent_binding * binding);
int bpg_net_agent_binding_connection_id_cmp(
    const struct bpg_net_agent_binding * l,
    const struct bpg_net_agent_binding * r);
void bpg_net_agent_binding_free_all(bpg_net_agent_t mgr);

/*client process*/
int bpg_net_client_ep_init(bpg_net_client_t client, net_ep_t ep);
bpg_pkg_t bpg_net_client_req_buf(bpg_net_client_t mgr);
int bpg_net_client_send(dp_req_t req, void * ctx, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
