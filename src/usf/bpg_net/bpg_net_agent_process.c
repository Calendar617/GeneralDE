#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_chanel.h"
#include "cpe/net/net_endpoint.h"
#include "gd/dp/dp_manage.h"
#include "gd/app/app_context.h"
#include "usf/bpg/bpg_req.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

static LPDRMETAENTRY s_totallen_entry;
static int s_totallen_start_pos = 0;
static int s_totallen_end_pos = 0;

static void bpg_net_agent_on_read(bpg_net_agent_t agent, net_ep_t ep) {
    bpg_req_t req_buf;

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

    if (s_totallen_start_pos == 0) {
        s_totallen_entry = dr_meta_find_entry_by_name(bpg_meta_pkghead(), "bodytotallen");
        if (s_totallen_entry == NULL) {
            s_totallen_start_pos = -1;
        }
        else {
            s_totallen_start_pos = dr_entry_data_start_pos(s_totallen_entry);
            s_totallen_end_pos = s_totallen_start_pos + dr_entry_element_size(s_totallen_entry);
        }
    }

    if (s_totallen_start_pos < 0) {
        CPE_ERROR(
            agent->m_em, "%s: ep %d: get bodytotallen entry fail!",
            bpg_net_agent_name(agent), (int)net_ep_id(ep));
        net_ep_close(ep);
        return;
    }

    assert(s_totallen_entry);

    while(1) {
        char * buf;
        size_t buf_size;
        int32_t package_input_size;
        ssize_t package_decode_size;

        buf_size = net_ep_size(ep);

        if (buf_size < s_totallen_end_pos) break;

        buf = net_ep_peek(ep, NULL, buf_size);
        if (buf == NULL) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: peek data fail, size=%d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)buf_size);
            net_ep_close(ep);
            break;
        }

        if (dr_entry_try_read_int32(&package_input_size, buf + s_totallen_start_pos, s_totallen_entry, agent->m_em) != 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: read package size fail!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep));
            net_ep_close(ep);
            break;
        }

        package_decode_size =
            dr_cvt_decode(
                bpg_net_agent_cvt(agent),
                bpg_meta_pkg(),
                bpg_req_pkg_data(req_buf),
                bpg_req_pkg_capacity(req_buf),
                buf, package_input_size, agent->m_em);
        net_ep_erase(ep, package_input_size);

        if (package_decode_size < 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: decode package fail!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep));
            continue;
        }

        if (bpg_req_pkg_data_set_size(req_buf, package_decode_size) != 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: bpg set size %d error!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), (int)package_decode_size);
            net_ep_close(ep);
            break;
        }

        if(agent->m_debug) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: read one request, cmd=%d, input-size=%d, package-size=%d!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep),
                bpg_req_cmd(req_buf), (int)package_input_size, (int)package_decode_size);
        }

        if (gd_dp_dispatch_by_numeric(bpg_req_cmd(req_buf), bpg_req_to_dp_req(req_buf), agent->m_em) != 0) {
            CPE_ERROR(
                agent->m_em, "%s: ep %d: dispatch cmd %d error!",
                bpg_net_agent_name(agent), (int)net_ep_id(ep), bpg_req_cmd(req_buf));
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

