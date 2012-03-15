#ifndef USF_BPG_NET_INTERNAL_OPS_H
#define USF_BPG_NET_INTERNAL_OPS_H
#include "cpe/net/net_types.h"
#include "bpg_net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void bpg_net_agent_accept(net_listener_t listener, net_ep_t ep, void * ctx);
bpg_req_t bpg_net_agent_req_buf(bpg_net_agent_t mgr);

#ifdef __cplusplus
}
#endif

#endif
