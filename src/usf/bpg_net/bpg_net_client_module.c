#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_types.h"

EXPORT_DIRECTIVE
int bpg_net_client_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_net_client_t bpg_net_client;
    const char * ip;
    short port;
    int accept_queue_size;

    ip = cfg_get_string(cfg, "ip", "");
    port = cfg_get_int16(cfg, "port", 0);
    accept_queue_size = cfg_get_int32(cfg, "accept-queue-size", 256);

    bpg_net_client =
        bpg_net_client_create(
            app, gd_app_module_name(module),
            ip, port,
            gd_app_alloc(app), gd_app_em(app));
    if (bpg_net_client == NULL) return -1;

    bpg_net_client->m_req_max_size =
        cfg_get_uint32(cfg, "req-max-size", bpg_net_client->m_req_max_size);

    bpg_net_client->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (bpg_net_client->m_debug) {
        CPE_INFO(
            gd_app_em(app),
            "%s: create: done. ip=%s, port=%u, req-max-size=%d",
            gd_app_module_name(module),
            ip, port, 
            (int)bpg_net_client->m_req_max_size);
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_net_client_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_net_client_t bpg_net_client;

    bpg_net_client = bpg_net_client_find_nc(app, gd_app_module_name(module));
    if (bpg_net_client) {
        bpg_net_client_free(bpg_net_client);
    }
}
