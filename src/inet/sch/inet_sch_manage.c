#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "cpe/net/net_manage.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "inet/sch/inet_sch_manage.h"
#include "inet_sch_internal_ops.h"

static void inet_sch_manage_clear(nm_node_t node);

static cpe_hash_string_buf s_inet_sch_manage_default_name = CPE_HS_BUF_MAKE("inet_sch_manage");

struct nm_node_type s_nm_node_type_inet_sch_manage = {
    "usf_inet_sch_manage",
    inet_sch_manage_clear
};

inet_sch_manage_t
inet_sch_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    inet_sch_manage_t mgr;
    nm_node_t mgr_node;

    if (name == NULL) name = cpe_hs_data((cpe_hash_string_t)&s_inet_sch_manage_default_name);

    mgr_node = nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct inet_sch_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (inet_sch_manage_t)nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_em = em;
    mgr->m_debug = 0;

    mgr->m_loop = net_mgr_ev_loop(gd_app_net_mgr(app));
    assert(mgr->m_loop);

    mgr->m_multi = curl_multi_init();
    if (mgr->m_multi == NULL) {
        CPE_ERROR(mgr->m_em, "create inet_sch_manage %s: curl_multi_init fail!", name);
        nm_node_free(mgr_node);
        return NULL;
    }

    curl_multi_setopt(mgr->m_multi, CURLMOPT_SOCKETFUNCTION, inet_sch_manage_sock_cb);
    curl_multi_setopt(mgr->m_multi, CURLMOPT_SOCKETDATA, mgr);
    curl_multi_setopt(mgr->m_multi, CURLMOPT_TIMERFUNCTION, inet_sch_manage_timer_update_cb);
    curl_multi_setopt(mgr->m_multi, CURLMOPT_TIMERDATA, mgr);

    nm_node_set_type(mgr_node, &s_nm_node_type_inet_sch_manage);

    return mgr;
}

static void inet_sch_manage_clear(nm_node_t node) {
    inet_sch_manage_t mgr;
    mgr = (inet_sch_manage_t)nm_node_data(node);

    curl_multi_cleanup(mgr->m_multi);
}

void inet_sch_manage_free(inet_sch_manage_t mgr) {
    nm_node_t mgr_node;
    assert(mgr);

    mgr_node = nm_node_from_data(mgr);
    if (nm_node_type(mgr_node) != &s_nm_node_type_inet_sch_manage) return;
    nm_node_free(mgr_node);
}

inet_sch_manage_t
inet_sch_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name)
{
    nm_node_t node;
    if (name == NULL) name = (cpe_hash_string_t)&s_inet_sch_manage_default_name;

    node = nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_inet_sch_manage) return NULL;
    return (inet_sch_manage_t)nm_node_data(node);
}

inet_sch_manage_t
inet_sch_manage_find_nc(
    gd_app_context_t app,
    const char * name)
{
    nm_node_t node;

    if (name == NULL) return inet_sch_manage_default(app);

    node = nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_inet_sch_manage) return NULL;
    return (inet_sch_manage_t)nm_node_data(node);
}

inet_sch_manage_t
inet_sch_manage_default(
    gd_app_context_t app)
{
    return inet_sch_manage_find(app, (cpe_hash_string_t)&s_inet_sch_manage_default_name);
}

gd_app_context_t inet_sch_manage_app(inet_sch_manage_t mgr) {
    return mgr->m_app;
}

const char * inet_sch_manage_name(inet_sch_manage_t mgr) {
    return nm_node_name(nm_node_from_data(mgr));
}

cpe_hash_string_t
inet_sch_manage_name_hs(inet_sch_manage_t mgr) {
    return nm_node_name_hs(nm_node_from_data(mgr));
}

EXPORT_DIRECTIVE
int inet_sch_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    inet_sch_manage_t inet_sch_manage;

    inet_sch_manage = inet_sch_manage_create(app, gd_app_module_name(module), gd_app_alloc(app), gd_app_em(app));
    if (inet_sch_manage == NULL) return -1;

    inet_sch_manage->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (inet_sch_manage->m_debug) {
        CPE_INFO(
            gd_app_em(app), "%s: create: done",
            inet_sch_manage_name(inet_sch_manage));
    }

    return 0;
}

EXPORT_DIRECTIVE
void inet_sch_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    inet_sch_manage_t inet_sch_manage;

    inet_sch_manage = inet_sch_manage_find_nc(app, gd_app_module_name(module));
    if (inet_sch_manage) {
        inet_sch_manage_free(inet_sch_manage);
    }
}

