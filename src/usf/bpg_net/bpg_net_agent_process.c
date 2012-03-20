#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_chanel.h"
#include "cpe/net/net_endpoint.h"
#include "gd/dp/dp_manage.h"
#include "gd/app/app_context.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

static void bpg_net_agent_on_read(bpg_net_agent_t agent, net_ep_t ep) {
    bpg_pkg_t req_buf;

    if(agent->m_debug) {
        CPE_INFO(
            agent->m_em, "%s: ep %d: on read",
            bpg_net_agent_name(agent), (int)net_ep_id(ep));
    }

    req_buf = bpg_net_agent_req_buf(agent);
    if (req_buf == NULL) {
        CPE_ERROR(
            agent->m_em, "%s: ep %d: get req buf fail!",
            bpg_net_agent_name(agent), (int)net_ep_id(ep));
        net_ep_close(ep);
        return;
    }

    while(1) {
        char * buf;
        size_t buf_size;
        size_t input_size;
        size_t output_size;
        dr_cvt_result_t cvt_result;

        buf_size = net_ep_size(ep);
        if (buf_size <= 0) break;

        buf = net_ep_peek(ep, NULL, buf_size);
        if (buf == NULL) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: peek data fail, size=%d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)buf_size);
            net_ep_close(ep);
            break;
        }

        input_size = buf_size;
        output_size = bpg_pkg_pkg_capacity(req_buf);

        cvt_result =
            dr_cvt_decode(
                bpg_pkg_base_cvt(req_buf),
                bpg_pkg_base_meta(req_buf),
                bpg_pkg_pkg_data(req_buf),
                &output_size,
                buf, &input_size, agent->m_em, agent->m_debug);
        if (cvt_result != dr_cvt_result_not_enough_input) {
            if(agent->m_debug) {
                CPE_ERROR(
                    agent->m_em, "%s: ep %d: not enough data, input size is %d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)buf_size);
            }
            break;
        }
        else if (cvt_result != dr_cvt_result_success) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: decode package fail, input size is %d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)buf_size);
            net_ep_close(ep);
            break;
        }
        net_ep_erase(ep, input_size);

        if(agent->m_debug) {
            CPE_INFO(
                agent->m_em, "%s: ep %d: decode one package, output-size=%d, buf-origin-size=%d left-size=%d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)output_size, (int)buf_size, (int)net_ep_size(ep));
        }

        if (bpg_pkg_pkg_data_set_size(req_buf, output_size) != 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: bpg set size %d error!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)output_size);
            net_ep_close(ep);
            break;
        }

        if(agent->m_debug) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: read one request, cmd=%d, input-size=%d, output-size=%d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep),
                bpg_pkg_cmd(req_buf), (int)input_size, (int)output_size);
        }

        if (gd_dp_dispatch_by_numeric(bpg_pkg_cmd(req_buf), bpg_pkg_to_dp_req(req_buf), agent->m_em) != 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: dispatch cmd %d error!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), bpg_pkg_cmd(req_buf));
        }
    }
}

static void bpg_net_agent_on_open(bpg_net_agent_t agent, net_ep_t ep) {
    if(agent->m_debug) {
        CPE_INFO(
            agent->m_em, "%s: ep %d: on open",
            bpg_net_agent_name(agent), (int)net_ep_id(ep));
    }

}

static void bpg_net_agent_on_close(bpg_net_agent_t agent, net_ep_t ep, net_ep_event_t event) {
    if(agent->m_debug) {
        CPE_INFO(
            agent->m_em, "%s: ep %d: on close, event=%d",
            bpg_net_agent_name(agent), (int)net_ep_id(ep), event);
    }
}

static void bpg_net_agent_process(net_ep_t ep, void * ctx, net_ep_event_t event) {
    bpg_net_agent_t agent = (bpg_net_agent_t)ctx;

    assert(agent);

    switch(event) {
    case net_ep_event_read:
        bpg_net_agent_on_read(agent, ep);
        break;
    case net_ep_event_open:
        bpg_net_agent_on_open(agent, ep);
        break;
    default:
        bpg_net_agent_on_close(agent, ep, event);
        break;
    }
}

static void bpg_net_agent_free_chanel_buf(net_chanel_t chanel, void * ctx) {
    bpg_net_agent_t agent = (bpg_net_agent_t)ctx;

    assert(agent);

    mem_free(agent->m_alloc, net_chanel_queue_buf(chanel));
}


void bpg_net_agent_accept(net_listener_t listener, net_ep_t ep, void * ctx) {
    bpg_net_agent_t agent = (bpg_net_agent_t)ctx;
    void * buf_r = NULL;
    void * buf_w = NULL;
    net_chanel_t chanel_r = NULL;
    net_chanel_t chanel_w = NULL;

    assert(agent);

    buf_r = mem_alloc(agent->m_alloc, 2048);
    buf_w = mem_alloc(agent->m_alloc, 2048);
    if (buf_r == NULL || buf_w == NULL) goto ERROR;

    chanel_r = net_chanel_queue_create(net_ep_mgr(ep), buf_r, 2048);
    if (chanel_r == NULL) goto ERROR;
    net_chanel_queue_set_close(chanel_r, bpg_net_agent_free_chanel_buf, agent);
    buf_r = NULL;

    chanel_w = net_chanel_queue_create(net_ep_mgr(ep), buf_w, 2048);
    if (chanel_w == NULL) goto ERROR;
    net_chanel_queue_set_close(chanel_w, bpg_net_agent_free_chanel_buf, agent);
    buf_w = NULL;

    net_ep_set_chanel_r(ep, chanel_r);
    chanel_r = NULL;

    net_ep_set_chanel_w(ep, chanel_w);
    chanel_w = NULL;

    net_ep_set_processor(ep, bpg_net_agent_process, agent);

    if(agent->m_debug) {
        CPE_INFO(
            agent->m_em, "%s: ep %d: accept success!",
            bpg_net_agent_name(agent), (int)net_ep_id(ep));
    }

    return;
ERROR:
    if (buf_r) mem_free(agent->m_alloc, buf_r);
    if (buf_w) mem_free(agent->m_alloc, buf_w);
    if (chanel_r) net_chanel_free(chanel_r);
    if (chanel_w) net_chanel_free(chanel_w);
    net_ep_close(ep);

    CPE_ERROR(
        agent->m_em, "%s: ep %d: accept fail!",
        bpg_net_agent_name(agent), (int)net_ep_id(ep));
}

