#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/logic/logic_manage.h"
#include "bpg_internal_ops.h"

static void bpg_manage_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_bpg_manager_default_name = CPE_HS_BUF_MAKE("bpg_manager");

struct gd_nm_node_type s_nm_node_type_bpg_manage = {
    "usf_bpg_manage",
    bpg_manage_clear
};

bpg_manage_t
bpg_manage_create(
    gd_app_context_t app,
    const char * name,
    logic_manage_t logic_mgr,
    error_monitor_t em)
{
    bpg_manage_t mgr;
    gd_nm_node_t mgr_node;

    assert(app);

    if (logic_mgr == 0) logic_mgr = logic_manage_default(app);
    if (logic_mgr == 0) return NULL;

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_bpg_manager_default_name);
    if (em == 0) em = gd_app_em(app);

    mgr_node = gd_nm_group_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_manage_t)gd_nm_node_data(mgr_node);

    mgr->m_app = app;
    mgr->m_logic_mgr = logic_mgr;
    mgr->m_em = em;
    mgr->m_flags = 0;

    mgr->m_ctx_capacity = 0;
    mgr->m_ctx_init = NULL;
    mgr->m_ctx_fini = NULL;
    mgr->m_ctx_ctx = NULL;

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_bpg_manage);

    return mgr;
}

static void bpg_manage_clear(gd_nm_node_t node) {
    bpg_manage_t mgr;
    mgr = (bpg_manage_t)gd_nm_node_data(node);
}

void bpg_manage_free(bpg_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_bpg_manage) return;
    gd_nm_node_free(mgr_node);
}

bpg_manage_t
bpg_manage_find(gd_app_context_t app, cpe_hash_string_t name) {
    gd_nm_node_t node;

    if (name == NULL) name = (cpe_hash_string_t)&s_bpg_manager_default_name;

    node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_manage) return NULL;
    return (bpg_manage_t)gd_nm_node_data(node);
}

bpg_manage_t
bpg_manage_find_nc(gd_app_context_t app, const char * name) {
    gd_nm_node_t node;

    if (name == NULL) return bpg_manage_default(app);

    node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_manage) return NULL;
    return (bpg_manage_t)gd_nm_node_data(node);
}

bpg_manage_t
bpg_manage_default(gd_app_context_t app) {
    return bpg_manage_find(app, (cpe_hash_string_t)&s_bpg_manager_default_name);
}

gd_app_context_t bpg_manage_app(bpg_manage_t mgr) {
    return mgr->m_app;
}

const char * bpg_manage_name(bpg_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_manage_name_hs(bpg_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

uint32_t bpg_manage_flags(bpg_manage_t mgr) {
    return mgr->m_flags;
}

void bpg_manage_flags_set(bpg_manage_t mgr, uint32_t flag) {
    mgr->m_flags = flag;
}

void bpg_manage_flag_enable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    mgr->m_flags |= flag;
}

void bpg_manage_flag_disable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    mgr->m_flags &= ~((uint32_t)flag);
}

int bpg_manage_flag_is_enable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    return mgr->m_flags & flag;
}

void bpg_manage_set_context_op(
    bpg_manage_t mgr,
    size_t ctx_capacity,
    bpg_logic_ctx_init_fun_t ctx_init,
    bpg_logic_ctx_fini_fun_t ctx_fini,
    void * ctx_ctx)
{
    mgr->m_ctx_capacity = ctx_capacity;
    mgr->m_ctx_init = ctx_init;
    mgr->m_ctx_fini = ctx_fini;
    mgr->m_ctx_ctx = ctx_ctx;
}

EXPORT_DIRECTIVE
int bpg_manager_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_manage_t bpg_manager;

    bpg_manager = bpg_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_manager == NULL) return -1;

    return 0;
}

EXPORT_DIRECTIVE
void bpg_manager_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_manage_t bpg_manager;

    bpg_manager = bpg_manage_find(app, gd_app_module_name_hs(module));
    if (bpg_manager) {
        bpg_manage_free(bpg_manager);
    }
}

