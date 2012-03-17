#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_connector.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg/bpg_req.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_ops.h"

struct gd_nm_node_type s_nm_node_type_bpg_net_client;

bpg_net_client_t
bpg_net_client_create(
    gd_app_context_t app,
    const char * name,
    const char * ip,
    short port,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    bpg_net_client_t mgr;
    gd_nm_node_t mgr_node;

    assert(name);
    assert(ip);

    if (em == 0) em = gd_app_em(app);

    mgr_node = gd_nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_net_client));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_net_client_t)gd_nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_em = em;
    mgr->m_req_max_size = 4 * 1024;
    mgr->m_req_buf = NULL;

    mgr->m_debug = 0;

    mgr->m_connector =
        net_connector_create(
            gd_app_net_mgr(app),
            name,
            ip,
            port);
    if (mgr->m_connector == NULL) {
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_bpg_net_client);

    return mgr;
}

static void bpg_net_client_clear(gd_nm_node_t node) {
    bpg_net_client_t mgr;
    mgr = (bpg_net_client_t)gd_nm_node_data(node);

    if (mgr->m_req_buf) {
        bpg_req_free(mgr->m_req_buf);
        mgr->m_req_buf = NULL;
    }

    if (mgr->m_cvt) {
        dr_cvt_free(mgr->m_cvt);
        mgr->m_cvt = NULL;
    }

    net_connector_free(mgr->m_connector);
}

void bpg_net_client_free(bpg_net_client_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_bpg_net_client) return;
    gd_nm_node_free(mgr_node);
}

bpg_net_client_t
bpg_net_client_find(gd_app_context_t app, cpe_hash_string_t name) {
    if (name == NULL) return NULL;

    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_net_client) return NULL;
    return (bpg_net_client_t)gd_nm_node_data(node);
}

bpg_net_client_t
bpg_net_client_find_nc(gd_app_context_t app, const char * name) {
    if (name == NULL) return NULL;

    gd_nm_node_t node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_net_client) return NULL;
    return (bpg_net_client_t)gd_nm_node_data(node);
}

gd_app_context_t bpg_net_client_app(bpg_net_client_t mgr) {
    return mgr->m_app;
}

const char * bpg_net_client_name(bpg_net_client_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_net_client_name_hs(bpg_net_client_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

int bpg_net_client_set_cvt(bpg_net_client_t mgr, const char * cvt) {
    dr_cvt_t new_cvt;

    if (mgr->m_cvt && strcmp(dr_cvt_name(mgr->m_cvt), cvt) == 0) return 0;

    new_cvt = dr_cvt_create(cvt);
    if (new_cvt == NULL) return -1;

    if (mgr->m_cvt) dr_cvt_free(mgr->m_cvt);

    mgr->m_cvt = new_cvt;
    return 0;
}

dr_cvt_t bpg_net_client_cvt(bpg_net_client_t mgr) {
    return mgr->m_cvt;
}

const char * bpg_net_client_cvt_name(bpg_net_client_t mgr) {
    return mgr->m_cvt ? dr_cvt_name(mgr->m_cvt) : "";
}

bpg_req_t
bpg_net_client_req_buf(bpg_net_client_t mgr) {
    if (mgr->m_req_buf) {
        if (bpg_req_pkg_capacity(mgr->m_req_buf) < mgr->m_req_max_size) {
            bpg_req_free(mgr->m_req_buf);
            mgr->m_req_buf = NULL;
        }
    }

    if (mgr->m_req_buf == NULL) {
        mgr->m_req_buf = bpg_req_create(mgr->m_app, mgr->m_req_max_size, NULL, 0);
    }

    return mgr->m_req_buf;
}

static void bpg_net_client_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_bpg_net_client = {
    "usf_bpg_net_client",
    bpg_net_client_clear
};

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
