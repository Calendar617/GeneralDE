#include <assert.h>
#include "bpg_net_internal_ops.h"

void bpg_net_agent_accept(net_listener_t listener, net_ep_t ep, void * ctx) {
    bpg_net_agent_t agent = (bpg_net_agent_t)ctx;

    assert(agent);
}

