#include "cpe/pal/pal_stdio.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/utils/buffer.h"
#include "cpe/net/net_endpoint.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

int bpg_net_agent_reply(dp_req_t req, void * ctx, error_monitor_t em) {
    bpg_net_agent_t agent;
    bpg_pkg_t pkg;
    size_t pkg_size;
    size_t write_size;
    net_ep_t ep;
    dr_cvt_result_t cvt_result;

    agent = (bpg_net_agent_t)ctx;
    pkg = bpg_pkg_from_dp_req(req);

    if (pkg == NULL) {
        CPE_ERROR(
            em, "%s: bpg_net_agent_reply: input req is not bpg_pkg!",
            bpg_net_agent_name(agent));

        if (agent->m_debug) {
            CPE_INFO(agent->m_em, "\n\n");
        }

        return 0;
    }

    if (agent->m_debug) {
        struct mem_buffer buffer;
        mem_buffer_init(&buffer, NULL);

        CPE_ERROR(
            agent->m_em,
            "%s: bpg_net_agent_reply: receive one response!\n"
            "%s",
            bpg_net_agent_name(agent),
            bpg_pkg_dump(pkg, &buffer));

        mem_buffer_clear(&buffer);
    }
    
    ep = bpg_net_agent_process_reply(
        agent,
        bpg_pkg_client_id(pkg),
        bpg_pkg_connection_id(pkg));
    if (ep == NULL) {
        CPE_ERROR(
            em, "%s: bpg_net_agent_reply: can`t detect ep from req!",
            bpg_net_agent_name(agent));
        return 0;
    }

    mem_buffer_set_size(&agent->m_rsp_buf, agent->m_req_max_size);
    pkg_size = bpg_pkg_pkg_data_size(pkg);
    write_size = mem_buffer_size(&agent->m_rsp_buf);
    cvt_result =
        dr_cvt_encode(
            bpg_pkg_base_cvt(pkg),
            bpg_pkg_base_meta(pkg),
            mem_buffer_make_continuous(&agent->m_rsp_buf, 0),
            &write_size,
            bpg_pkg_pkg_data(pkg),
            &pkg_size,
            agent->m_em, agent->m_debug);
    if (cvt_result != dr_cvt_result_success) {
        CPE_ERROR(
            agent->m_em, "%s: bpg_net_agent_reply: encode package for send fail!",
            bpg_net_agent_name(agent));

        if (agent->m_debug) {
            CPE_INFO(agent->m_em, "\n\n");
        }

        return 0;
    }

    if (net_ep_send(ep, mem_buffer_make_continuous(&agent->m_rsp_buf, 0), write_size) != 0) {
        CPE_ERROR(
            agent->m_em, "%s: bpg_net_agent_reply: send data fail, write_size=%d!",
            bpg_net_agent_name(agent), (int)write_size);
        net_ep_close(ep);

        if (agent->m_debug) {
            CPE_INFO(agent->m_em, "\n\n");
        }

        return 0;
    }
    
    if (agent->m_debug) {
        CPE_ERROR(
            agent->m_em,
            "%s: bpg_net_agent_reply: send one response, write-size=" FMT_SIZE_T " !\n\n",
            bpg_net_agent_name(agent), write_size);
    }

    return 0;
}
