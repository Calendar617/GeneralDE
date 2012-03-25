#include <assert.h>
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_chanel.h"
#include "cpe/net/net_endpoint.h"
#include "gd/dp/dp_manage.h"
#include "gd/app/app_context.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_dsp.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_ops.h"

static void bpg_net_client_on_read(bpg_net_client_t client, net_ep_t ep) {
    bpg_pkg_t req_buf;

    if(client->m_debug) {
        CPE_INFO(
            client->m_em, "%s: ep %d: on read",
            bpg_net_client_name(client), (int)net_ep_id(ep));
    }

    req_buf = bpg_net_client_req_buf(client);
    if (req_buf == NULL) {
        CPE_ERROR(
            client->m_em, "%s: ep %d: get req buf fail!",
            bpg_net_client_name(client), (int)net_ep_id(ep));
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
                client->m_em, "%s: ep %d: peek data fail, size=%d!",
                bpg_net_client_name(client), (int)net_ep_id(ep), (int)buf_size);
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
                buf, &input_size, client->m_em, client->m_debug);
        if (cvt_result == dr_cvt_result_not_enough_input) {
            if(client->m_debug) {
                CPE_ERROR(
                    client->m_em, "%s: ep %d: not enough data, input size is %d!",
                bpg_net_client_name(client), (int)net_ep_id(ep), (int)buf_size);
            }
            break;
        }
        else if (cvt_result != dr_cvt_result_success) {
            CPE_ERROR(
                client->m_em, "%s: ep %d: decode package fail, input size is %d!",
                bpg_net_client_name(client), (int)net_ep_id(ep), (int)buf_size);
            net_ep_close(ep);
            break;
        }
        net_ep_erase(ep, input_size);

        if(client->m_debug) {
            CPE_INFO(
                client->m_em, "%s: ep %d: decode one package, output-size=%d, buf-origin-size=%d left-size=%d!",
                bpg_net_client_name(client), (int)net_ep_id(ep), (int)output_size, (int)buf_size, (int)net_ep_size(ep));
        }

        if (bpg_pkg_pkg_data_set_size(req_buf, output_size) != 0) {
            CPE_ERROR(
                client->m_em, "%s: ep %d: bpg set size %d error!",
                bpg_net_client_name(client), (int)net_ep_id(ep), (int)output_size);
            net_ep_close(ep);
            break;
        }

        if(client->m_debug) {
            CPE_ERROR(
                client->m_em, "%s: ep %d: read one request, cmd=%d, input-size=%d, output-size=%d!",
                bpg_net_client_name(client), (int)net_ep_id(ep),
                bpg_pkg_cmd(req_buf), (int)input_size, (int)output_size);
        }

        if (bpg_pkg_dsp_dispatch(client->m_rsp_dsp, req_buf, client->m_em) != 0) {
            CPE_ERROR(
                client->m_em, "%s: ep %d: dispatch cmd %d error!",
                bpg_net_client_name(client), (int)net_ep_id(ep), bpg_pkg_cmd(req_buf));
        }
    }
}

static void bpg_net_client_on_open(bpg_net_client_t client, net_ep_t ep) {
    if(client->m_debug) {
        CPE_INFO(
            client->m_em, "%s: ep %d: on open",
            bpg_net_client_name(client), (int)net_ep_id(ep));
    }

}

static void bpg_net_client_on_close(bpg_net_client_t client, net_ep_t ep, net_ep_event_t event) {
    if(client->m_debug) {
        CPE_INFO(
            client->m_em, "%s: ep %d: on close, event=%d",
            bpg_net_client_name(client), (int)net_ep_id(ep), event);
    }
}

static void bpg_net_client_process(net_ep_t ep, void * ctx, net_ep_event_t event) {
    bpg_net_client_t client = (bpg_net_client_t)ctx;

    assert(client);

    switch(event) {
    case net_ep_event_read:
        bpg_net_client_on_read(client, ep);
        break;
    case net_ep_event_open:
        bpg_net_client_on_open(client, ep);
        break;
    default:
        bpg_net_client_on_close(client, ep, event);
        break;
    }
}

static void bpg_net_client_free_chanel_buf(net_chanel_t chanel, void * ctx) {
    bpg_net_client_t client = (bpg_net_client_t)ctx;

    assert(client);

    mem_free(client->m_alloc, net_chanel_queue_buf(chanel));
}

int bpg_net_client_ep_init(bpg_net_client_t client, net_ep_t ep) {
    void * buf_r = NULL;
    void * buf_w = NULL;
    net_chanel_t chanel_r = NULL;
    net_chanel_t chanel_w = NULL;

    assert(client);

    buf_r = mem_alloc(client->m_alloc, 2048);
    buf_w = mem_alloc(client->m_alloc, 2048);
    if (buf_r == NULL || buf_w == NULL) goto ERROR;

    chanel_r = net_chanel_queue_create(net_ep_mgr(ep), buf_r, 2048);
    if (chanel_r == NULL) goto ERROR;
    net_chanel_queue_set_close(chanel_r, bpg_net_client_free_chanel_buf, client);
    buf_r = NULL;

    chanel_w = net_chanel_queue_create(net_ep_mgr(ep), buf_w, 2048);
    if (chanel_w == NULL) goto ERROR;
    net_chanel_queue_set_close(chanel_w, bpg_net_client_free_chanel_buf, client);
    buf_w = NULL;

    net_ep_set_chanel_r(ep, chanel_r);
    chanel_r = NULL;

    net_ep_set_chanel_w(ep, chanel_w);
    chanel_w = NULL;

    net_ep_set_processor(ep, bpg_net_client_process, client);

    if(client->m_debug) {
        CPE_INFO(
            client->m_em, "%s: ep %d: init success!",
            bpg_net_client_name(client), (int)net_ep_id(ep));
    }

    return 0;
ERROR:
    if (buf_r) mem_free(client->m_alloc, buf_r);
    if (buf_w) mem_free(client->m_alloc, buf_w);
    if (chanel_r) net_chanel_free(chanel_r);
    if (chanel_w) net_chanel_free(chanel_w);
    net_ep_close(ep);

    CPE_ERROR(
        client->m_em, "%s: ep %d: init fail!",
        bpg_net_client_name(client), (int)net_ep_id(ep));

    return -1;
}

