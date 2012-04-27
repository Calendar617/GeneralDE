#include "cpe/pal/pal_stdio.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/utils/buffer.h"
#include "cpe/net/net_endpoint.h"
#include "cpe/net/net_connector.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_ops.h"

int bpg_net_client_send(dp_req_t req, void * ctx, error_monitor_t em) {
    bpg_net_client_t client;
    bpg_pkg_t pkg;
    size_t pkg_size;
    size_t write_size;
    net_ep_t ep;
    dr_cvt_result_t cvt_result;
    
    client = (bpg_net_client_t)ctx;
    pkg = bpg_pkg_from_dp_req(req);

    if (pkg == NULL) {
        CPE_ERROR(
            em, "%s: bpg_net_client_reply: input req is not bpg_pkg!",
            bpg_net_client_name(client));
        return 0;
    }

    if (client->m_debug) {
        struct mem_buffer buffer;
        mem_buffer_init(&buffer, NULL);

        CPE_ERROR(
            client->m_em,
            "%s: bpg_net_client_send: send one request!\n"
            "%s",
            bpg_net_client_name(client),
            bpg_pkg_dump(pkg, &buffer));

        mem_buffer_clear(&buffer);
    }
    
    ep = net_connector_ep(client->m_connector);

    mem_buffer_set_size(&client->m_send_encode_buf, client->m_req_max_size);
    pkg_size = bpg_pkg_pkg_data_size(pkg);
    write_size = mem_buffer_size(&client->m_send_encode_buf);
    cvt_result =
        dr_cvt_encode(
            bpg_pkg_base_cvt(pkg),
            bpg_pkg_base_meta(pkg),
            mem_buffer_make_continuous(&client->m_send_encode_buf, 0),
            &write_size,
            bpg_pkg_pkg_data(pkg),
            &pkg_size,
            client->m_em, client->m_debug >= 2 ? 1 : 0);
    if (cvt_result != dr_cvt_result_success) {
        CPE_ERROR(
            client->m_em, "%s: bpg_net_client_send: encode package for send fail!",
            bpg_net_client_name(client));
        return 0;
    }

    if (net_ep_send(ep, mem_buffer_make_continuous(&client->m_send_encode_buf, 0), write_size) != 0) {
        CPE_ERROR(
            client->m_em, "%s: bpg_net_client_send: send data fail, write_size=" FMT_SIZE_T "!",
            bpg_net_client_name(client), write_size);
        net_ep_close(ep);
        return 0;
    }
    
    if (client->m_debug) {
        CPE_ERROR(
            client->m_em,
            "%s: bpg_net_client_send: send one request, write-size=" FMT_SIZE_T "!",
            bpg_net_client_name(client), write_size);
    }

    return 0;
}
