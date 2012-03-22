#include <assert.h>
#include "cpe/pal/pal_stdio.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_listener.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/dp/dp_responser.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

struct gd_nm_node_type s_nm_node_type_bpg_net_agent;

bpg_net_agent_t
bpg_net_agent_create(
    gd_app_context_t app,
    bpg_pkg_manage_t pkg_manage,
    const char * name,
    const char * ip,
    short port,
    int acceptQueueSize,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    bpg_net_agent_t mgr;
    gd_nm_node_t mgr_node;
    char name_buf[128];

    assert(app);
    assert(pkg_manage);
    assert(name);
    assert(ip);

    if (em == 0) em = gd_app_em(app);

    mgr_node = gd_nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_net_agent));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_net_agent_t)gd_nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_pkg_manage = pkg_manage;
    mgr->m_em = em;
    mgr->m_req_max_size = 4 * 1024;
    mgr->m_req_buf = NULL;
    mgr->m_debug = 0;

    mem_buffer_init(&mgr->m_rsp_buf, alloc);

    if (cpe_hash_table_init(
            &mgr->m_cliensts,
            alloc,
            (cpe_hash_fun_t) bpg_net_agent_binding_client_id_hash,
            (cpe_hash_cmp_t) bpg_net_agent_binding_client_id_cmp,
            CPE_HASH_OBJ2ENTRY(bpg_net_agent_binding, m_hh_client),
            -1) != 0)
    {
        mem_buffer_clear(&mgr->m_rsp_buf);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_connections,
            alloc,
            (cpe_hash_fun_t) bpg_net_agent_binding_connection_id_hash,
            (cpe_hash_cmp_t) bpg_net_agent_binding_connection_id_cmp,
            CPE_HASH_OBJ2ENTRY(bpg_net_agent_binding, m_hh_connection),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_cliensts);
        mem_buffer_clear(&mgr->m_rsp_buf);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    snprintf(name_buf, sizeof(name_buf), "%s.reply-rsp", name);
    mgr->m_reply_rsp = gd_dp_rsp_create(gd_app_dp_mgr(app), name_buf);
    if (mgr->m_reply_rsp == NULL) {
        cpe_hash_table_fini(&mgr->m_cliensts);
        cpe_hash_table_fini(&mgr->m_connections);
        mem_buffer_clear(&mgr->m_rsp_buf);
        gd_nm_node_free(mgr_node);
        return NULL;
    }
    gd_dp_rsp_set_processor(mgr->m_reply_rsp, bpg_net_agent_reply, mgr);

    mgr->m_listener =
        net_listener_create(
            gd_app_net_mgr(app),
            name,
            ip,
            port,
            acceptQueueSize,
            bpg_net_agent_accept,
            mgr);
    if (mgr->m_listener == NULL) {
        cpe_hash_table_fini(&mgr->m_cliensts);
        cpe_hash_table_fini(&mgr->m_connections);
        mem_buffer_clear(&mgr->m_rsp_buf);
        gd_dp_rsp_free(mgr->m_reply_rsp);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_bpg_net_agent);

    return mgr;
}

static void bpg_net_agent_clear(gd_nm_node_t node) {
    bpg_net_agent_t mgr;
    mgr = (bpg_net_agent_t)gd_nm_node_data(node);

    bpg_net_agent_binding_free_all(mgr);

    mem_buffer_clear(&mgr->m_rsp_buf);
    cpe_hash_table_fini(&mgr->m_cliensts);
    cpe_hash_table_fini(&mgr->m_connections);

    gd_dp_rsp_free(mgr->m_reply_rsp);
    mgr->m_reply_rsp = NULL;

    if (mgr->m_req_buf) {
        bpg_pkg_free(mgr->m_req_buf);
        mgr->m_req_buf = NULL;
    }

    net_listener_free(mgr->m_listener);
}

void bpg_net_agent_free(bpg_net_agent_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_bpg_net_agent) return;
    gd_nm_node_free(mgr_node);
}

bpg_net_agent_t
bpg_net_agent_find(gd_app_context_t app, cpe_hash_string_t name) {
    if (name == NULL) return NULL;

    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_net_agent) return NULL;
    return (bpg_net_agent_t)gd_nm_node_data(node);
}

bpg_net_agent_t
bpg_net_agent_find_nc(gd_app_context_t app, const char * name) {
    if (name == NULL) return NULL;

    gd_nm_node_t node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_net_agent) return NULL;
    return (bpg_net_agent_t)gd_nm_node_data(node);
}

gd_app_context_t bpg_net_agent_app(bpg_net_agent_t mgr) {
    return mgr->m_app;
}

const char * bpg_net_agent_name(bpg_net_agent_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_net_agent_name_hs(bpg_net_agent_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

short bpg_net_agent_port(bpg_net_agent_t svr) {
    return net_listener_using_port(svr->m_listener);
}

bpg_pkg_t
bpg_net_agent_req_buf(bpg_net_agent_t mgr) {
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

static void bpg_net_agent_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_bpg_net_agent = {
    "usf_bpg_net_agent",
    bpg_net_agent_clear
};
