#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_connector.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_client.h"
#include "bpg_net_internal_ops.h"

struct gd_nm_node_type s_nm_node_type_bpg_net_client;

bpg_net_client_t
bpg_net_client_create(
    gd_app_context_t app,
    bpg_pkg_manage_t pkg_manage,
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
    mgr->m_pkg_manage = pkg_manage;
    mgr->m_em = em;
    mgr->m_req_max_size = 4 * 1024;
    mgr->m_req_buf = NULL;

    mgr->m_debug = 0;

    mgr->m_connector =
        net_connector_create_with_ep(
            gd_app_net_mgr(app),
            name,
            ip,
            port);
    if (mgr->m_connector == NULL) {
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (bpg_net_client_ep_init(mgr, net_connector_ep(mgr->m_connector)) != 0) {
        net_connector_free(mgr->m_connector);
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
        bpg_pkg_free(mgr->m_req_buf);
        mgr->m_req_buf = NULL;
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

bpg_pkg_t
bpg_net_client_req_buf(bpg_net_client_t mgr) {
    if (mgr->m_req_buf) {
        if (bpg_pkg_pkg_capacity(mgr->m_req_buf) < mgr->m_req_max_size) {
            bpg_pkg_free(mgr->m_req_buf);
            mgr->m_req_buf = NULL;
        }
    }

    if (mgr->m_req_buf == NULL) {
        mgr->m_req_buf = bpg_pkg_create(mgr->m_pkg_manage, mgr->m_req_max_size, NULL, 0);
    }

    return mgr->m_req_buf;
}

net_connector_t bpg_net_client_connector(bpg_net_client_t mgr) {
    return mgr->m_connector;
}

static void bpg_net_client_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_bpg_net_client = {
    "usf_bpg_net_client",
    bpg_net_client_clear
};
