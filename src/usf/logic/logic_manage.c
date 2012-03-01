#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/logic/logic_manage.h"
#include "logic_internal_ops.h"

static void logic_manage_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_logic_manager_default_name = CPE_HS_BUF_MAKE("logic_manager");

struct gd_nm_node_type s_nm_node_type_logic_manage = {
    "usf_logic_manage",
    logic_manage_clear
};

logic_manage_t
logic_manage_create(
    const char * name,
    gd_app_context_t app,
    mem_allocrator_t alloc)
{
    logic_manage_t mgr;
    gd_nm_node_t mgr_node;

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_logic_manager_default_name);

    mgr_node = gd_nm_instance_create(gd_app_nm_mgr(app), name, sizeof(struct logic_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (logic_manage_t)gd_nm_node_data(mgr_node);
    mgr->m_alloc = alloc;
    mgr->m_app = app;
    mgr->m_context_id = 0;
    mgr->m_require_id = 0;

    if (cpe_hash_table_init(
            &mgr->m_require_types,
            alloc,
            (cpe_hash_fun_t) logic_require_type_hash,
            (cpe_hash_cmp_t) logic_require_type_cmp,
            CPE_HASH_OBJ2ENTRY(logic_require_type, m_hh),
            -1) != 0)
    {
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_requires,
            alloc,
            (cpe_hash_fun_t) logic_require_hash,
            (cpe_hash_cmp_t) logic_require_cmp,
            CPE_HASH_OBJ2ENTRY(logic_require, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_require_types);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_datas,
            alloc,
            (cpe_hash_fun_t) logic_data_hash,
            (cpe_hash_cmp_t) logic_data_cmp,
            CPE_HASH_OBJ2ENTRY(logic_data, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_require_types);
        cpe_hash_table_fini(&mgr->m_requires);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_contexts,
            alloc,
            (cpe_hash_fun_t) logic_context_hash,
            (cpe_hash_cmp_t) logic_context_cmp,
            CPE_HASH_OBJ2ENTRY(logic_context, m_hh),
            -1) != 0)
    {
        cpe_hash_table_fini(&mgr->m_require_types);
        cpe_hash_table_fini(&mgr->m_requires);
        cpe_hash_table_fini(&mgr->m_datas);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_logic_manage);

    return mgr;
}

static void logic_manage_clear(gd_nm_node_t node) {
    logic_manage_t mgr;
    mgr = (logic_manage_t)gd_nm_node_data(node);

    logic_require_free_all(mgr);
    logic_data_free_all(mgr);
    logic_context_free_all(mgr);
    logic_require_type_free_all(mgr);

    cpe_hash_table_fini(&mgr->m_contexts);
    cpe_hash_table_fini(&mgr->m_requires);
    cpe_hash_table_fini(&mgr->m_datas);
    cpe_hash_table_fini(&mgr->m_require_types);
}

void logic_manage_free(logic_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_logic_manage) return;
    gd_nm_node_free(mgr_node);
}

logic_manage_t
logic_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name)
{
    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_logic_manage) return NULL;
    return (logic_manage_t)gd_nm_node_data(node);
}

logic_manage_t
logic_manage_default(
    gd_app_context_t app)
{
    return logic_manage_find(app, (cpe_hash_string_t)&s_logic_manager_default_name);
}

gd_app_context_t logic_manage_app(logic_manage_t mgr) {
    return mgr->m_app;
}

const char * logic_manage_name(logic_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
logic_manage_name_hs(logic_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

EXPORT_DIRECTIVE
int logic_manager_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    logic_manage_t logic_manager;

    logic_manager = logic_manage_create(gd_app_module_name(module), app, gd_app_alloc(app));
    if (logic_manager == NULL) return -1;

    return 0;
}

EXPORT_DIRECTIVE
void logic_manager_app_fini(gd_app_context_t app, gd_app_module_t module) {
    logic_manage_t logic_manager;

    logic_manager = logic_manage_find(app, gd_app_module_name_hs(module));
    if (logic_manager) {
        logic_manage_free(logic_manager);
    }
}

