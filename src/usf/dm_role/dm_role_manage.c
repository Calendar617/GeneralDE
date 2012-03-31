#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/dm_role/dm_role_manage.h"
#include "dm_role_internal_types.h"

struct nm_node_type s_nm_node_type_dm_role_manage;
cpe_hash_string_t s_dm_role_manage_default_name;

dm_role_manage_t
dm_role_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em)
{
    dm_role_manage_t mgr;
    nm_node_t mgr_node;

    assert(app);
    assert(name);

    if (em == NULL) em = gd_app_em(app);

    mgr_node = nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct dm_role_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (dm_role_manage_t)nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_em = em;

    mgr->m_debug = 0;

    mgr->m_metalib = NULL;
    mgr->m_role_meta = NULL;
    mgr->m_role_id_entry = NULL;

    mgr->m_id_index = NULL;

    /* struct dm_role_index * m_id_index; */
    /* struct cpe_hash_table m_indexes; */

    nm_node_set_type(mgr_node, &s_nm_node_type_dm_role_manage);

    return mgr;
}

static void dm_role_manage_clear(nm_node_t node) {
    dm_role_manage_t mgr;
    mgr = (dm_role_manage_t)nm_node_data(node);
}

void dm_role_manage_free(dm_role_manage_t mgr) {
    nm_node_t mgr_node;
    assert(mgr);

    mgr_node = nm_node_from_data(mgr);
    if (nm_node_type(mgr_node) != &s_nm_node_type_dm_role_manage) return;
    nm_node_free(mgr_node);
}

dm_role_manage_t
dm_role_manage_find(gd_app_context_t app, cpe_hash_string_t name) {
    if (name == NULL) name = s_dm_role_manage_default_name;

    nm_node_t node = nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_dm_role_manage) return NULL;
    return (dm_role_manage_t)nm_node_data(node);
}

dm_role_manage_t
dm_role_manage_find_nc(gd_app_context_t app, const char * name) {
    if (name == NULL) return dm_role_manage_default(app);

    nm_node_t node = nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_dm_role_manage) return NULL;
    return (dm_role_manage_t)nm_node_data(node);
}

dm_role_manage_t
dm_role_manage_default(gd_app_context_t app) {
    return dm_role_manage_find(app, s_dm_role_manage_default_name);
}

gd_app_context_t dm_role_manage_app(dm_role_manage_t mgr) {
    return mgr->m_app;
}

const char * dm_role_manage_name(dm_role_manage_t mgr) {
    return nm_node_name(nm_node_from_data(mgr));
}

cpe_hash_string_t
dm_role_manage_name_hs(dm_role_manage_t mgr) {
    return nm_node_name_hs(nm_node_from_data(mgr));
}

CPE_HS_DEF_VAR(s_dm_role_manage_default_name, "dm_role_manage");

struct nm_node_type s_nm_node_type_dm_role_manage = {
    "usf_dm_role_manage",
    dm_role_manage_clear
};

EXPORT_DIRECTIVE
int dm_role_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    dm_role_manage_t dm_role_manage;

    dm_role_manage = dm_role_manage_create(
        app,
        gd_app_module_name(module),
        gd_app_alloc(app),
        gd_app_em(app));
    if (dm_role_manage == NULL) return -1;

    dm_role_manage->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (dm_role_manage->m_debug) {
        CPE_INFO(
            gd_app_em(app),
            "%s: create: done.",
            gd_app_module_name(module));
    }

    return 0;
}

EXPORT_DIRECTIVE
void dm_role_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    dm_role_manage_t dm_role_manage;

    dm_role_manage = dm_role_manage_find_nc(app, gd_app_module_name(module));
    if (dm_role_manage) {
        dm_role_manage_free(dm_role_manage);
    }
}
