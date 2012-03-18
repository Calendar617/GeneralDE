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
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_ops.h"

static void bpg_net_client_on_read(bpg_net_client_t client, net_ep_t ep) {
    bpg_req_t req_buf;

    if(client->m_debug) {
        CPE_INFO(
            client->m_em, "%s: ep %d: on read",
            bpg_net_client_name(client), (int)net_ep_id(ep));
    }

    /* req_buf = bpg_net_client_req_buf(client); */
    /* if (req_buf == NULL) { */
    /*     CPE_ERROR( */
    /*         client->m_em, "%s: ep %d: get req buf fail!", */
    /*         bpg_net_client_name(client), (int)net_ep_id(ep)); */
    /*     net_ep_close(ep); */
    /*     return; */
    /* } */

    /* if (s_totallen_start_pos == 0) { */
    /*     s_totallen_entry = dr_meta_find_entry_by_name(bpg_meta_pkghead(), "bodytotallen"); */
    /*     if (s_totallen_entry == NULL) { */
    /*         s_totallen_start_pos = -1; */
    /*     } */
    /*     else { */
    /*         s_totallen_start_pos = dr_entry_data_start_pos(s_totallen_entry); */
    /*         s_totallen_end_pos = s_totallen_start_pos + dr_entry_element_size(s_totallen_entry); */
    /*     } */
    /* } */

    /* if (s_totallen_start_pos < 0) { */
    /*     CPE_ERROR( */
    /*         client->m_em, "%s: ep %d: get bodytotallen entry fail!", */
    /*         bpg_net_client_name(client), (int)net_ep_id(ep)); */
    /*     net_ep_close(ep); */
    /*     return; */
    /* } */

    /* assert(s_totallen_entry); */

    /* while(1) { */
    /*     char * buf; */
    /*     size_t buf_size; */
    /*     int32_t package_input_size; */
    /*     ssize_t package_decode_size; */

    /*     buf_size = net_ep_size(ep); */

    /*     if (buf_size < s_totallen_end_pos) break; */

    /*     buf = net_ep_peek(ep, NULL, buf_size); */
    /*     if (buf == NULL) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: peek data fail, size=%d!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep), (int)buf_size); */
    /*         net_ep_close(ep); */
    /*         break; */
    /*     } */

    /*     if (dr_entry_try_read_int32(&package_input_size, buf + s_totallen_start_pos, s_totallen_entry, client->m_em) != 0) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: read package size fail!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep)); */
    /*         net_ep_close(ep); */
    /*         break; */
    /*     } */

    /*     package_decode_size = */
    /*         dr_cvt_decode( */
    /*             bpg_net_client_cvt(client), */
    /*             bpg_meta_pkg(), */
    /*             bpg_req_pkg_data(req_buf), */
    /*             bpg_req_pkg_capacity(req_buf), */
    /*             buf, package_input_size, client->m_em); */
    /*     net_ep_erase(ep, package_input_size); */

    /*     if (package_decode_size < 0) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: decode package fail!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep)); */
    /*         continue; */
    /*     } */

    /*     if (bpg_req_pkg_data_set_size(req_buf, package_decode_size) != 0) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: bpg set size %d error!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep), (int)package_decode_size); */
    /*         net_ep_close(ep); */
    /*         break; */
    /*     } */

    /*     if(client->m_debug) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: read one request, cmd=%d, input-size=%d, package-size=%d!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep), */
    /*             bpg_req_cmd(req_buf), (int)package_input_size, (int)package_decode_size); */
    /*     } */

    /*     if (gd_dp_dispatch_by_numeric(bpg_req_cmd(req_buf), bpg_req_to_dp_req(req_buf), client->m_em) != 0) { */
    /*         CPE_ERROR( */
    /*             client->m_em, "%s: ep %d: dispatch cmd %d error!", */
    /*             bpg_net_client_name(client), (int)net_ep_id(ep), bpg_req_cmd(req_buf)); */
    /*     } */
    /* } */
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

