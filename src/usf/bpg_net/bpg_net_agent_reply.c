#include "cpe/utils/buffer.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

int bpg_net_agent_reply(gd_dp_req_t req, void * ctx, error_monitor_t em) {
    bpg_net_agent_t agent;
    bpg_pkg_t pkg;
    size_t write_size;
    net_ep_t ep;

    agent = (bpg_net_agent_t)ctx;
    pkg = bpg_pkg_from_dp_req(req);

    if (pkg == NULL) {
        CPE_ERROR(
            em, "%s: bpg_net_agent_reply: input req is not bpg_pkg!",
            bpg_net_agent_name(agent));
        return 0;
    }

    ep = NULL;

    if (ep == NULL && bpg_pkg_connection_id(pkg) != BPG_INVALID_CONNECTION_ID) {
    }

    if (ep == NULL) {
        CPE_ERROR(
            em, "%s: bpg_net_agent_reply: can`t detect ep from req!",
            bpg_net_agent_name(agent));
        return 0;
    }
    
    if (agent->m_debug) {
        struct mem_buffer buffer;
        mem_buffer_init(&buffer, NULL);

        CPE_ERROR(
            agent->m_em,
            "%s: ep %d: send one response, cmd=%d, write-size=%d!\n"
            "%s"
            "\n\n",
            bpg_net_agent_name(agent), (int)0/*net_ep_id(ep)*/,
            bpg_pkg_cmd(pkg), (int)write_size,
            bpg_pkg_dump(pkg, &buffer));

        mem_buffer_clear(&buffer);
    }

    return 0;
}
