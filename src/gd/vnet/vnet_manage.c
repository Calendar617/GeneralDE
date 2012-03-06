#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/vnet/vnet_manage.h"
#include "vnet_internal_ops.h"

static void vnet_manage_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_vnet_manage = {
    "gd_vnet_manage",
    vnet_manage_clear
};

vnet_manage_t
vnet_manage_create(
    const char * name,
    gd_app_context_t app,
    mem_allocrator_t alloc)
{
    vnet_manage_t mgr;
    gd_nm_node_t mgr_node;

    mgr_node = gd_nm_group_create(gd_app_nm_mgr(app), name, sizeof(struct vnet_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (vnet_manage_t)gd_nm_node_data(mgr_node);

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_vnet_manage);

    return mgr;
}

static void vnet_manage_clear(gd_nm_node_t node) {
    vnet_manage_t mgr;
    mgr = (vnet_manage_t)gd_nm_node_data(node);
}

void vnet_manage_free(vnet_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_vnet_manage) return;
    gd_nm_node_free(mgr_node);
}

vnet_manage_t
vnet_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name)
{
    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_vnet_manage) return NULL;
    return (vnet_manage_t)gd_nm_node_data(node);
}

static cpe_hash_string_buf s_vnet_manager_default_name = CPE_HS_BUF_MAKE("vnet_manager");

vnet_manage_t
vnet_manage_default(
    gd_app_context_t app)
{
    return vnet_manage_find(app, (cpe_hash_string_t)&s_vnet_manager_default_name);
}

gd_app_context_t vnet_manage_app(vnet_manage_t mgr) {
    return mgr->m_app;
}

const char * vnet_manage_name(vnet_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
vnet_manage_name_hs(vnet_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

EXPORT_DIRECTIVE
int vnet_manager_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    vnet_manage_t vnet_manager;

    vnet_manager = vnet_manage_create(gd_app_module_name(module), app, gd_app_alloc(app));
    if (vnet_manager == NULL) return -1;

    return 0;
}

EXPORT_DIRECTIVE
void vnet_manager_app_fini(gd_app_context_t app, gd_app_module_t module) {
    vnet_manage_t vnet_manager;

    vnet_manager = vnet_manage_find(app, gd_app_module_name_hs(module));
    if (vnet_manager) {
        vnet_manage_free(vnet_manager);
    }
}

