#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/net/net_connector.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "bpg_net_internal_types.h"

EXPORT_DIRECTIVE
int bpg_net_client_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_net_client_t bpg_net_client;
    const char * ip;
    short port;
    bpg_pkg_manage_t pkg_manage;

    pkg_manage = bpg_pkg_manage_find_nc(app, cfg_get_string(cfg, "pkg-manage", NULL));
    if (pkg_manage == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: pkg-manage %s not exist!",
            gd_app_module_name(module),
            cfg_get_string(cfg, "pkg-manage", "default"));
        return -1;
    }

    ip = cfg_get_string(cfg, "ip", "");
    port = cfg_get_int16(cfg, "port", 0);

    bpg_net_client =
        bpg_net_client_create(
            app, pkg_manage, gd_app_module_name(module),
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

    if (cfg_get_int32(cfg, "auto-enable", 0)) {
        if (net_connector_enable(bpg_net_client_connector(bpg_net_client)) == 0) {
            if (bpg_net_client->m_debug) {
                CPE_INFO(
                    gd_app_em(app),
                    "%s: create: auto-enable success!",
                    gd_app_module_name(module));
            }
        }
        else {
            CPE_ERROR(
                gd_app_em(app),
                "%s: create: auto-enable fail!",
                gd_app_module_name(module));
            bpg_net_client_free(bpg_net_client);
            return -1;
        }
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
