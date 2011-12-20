#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_read.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_log.h"
#include "gd/app/app_context.h"
#include "app_internal_ops.h"


static struct gd_nm_node_type g_module_root_group = {
    "app_module_root",
    gd_nm_group_free_members
};

static struct gd_nm_node_type g_module_group = {
    "app_module",
    gd_nm_group_free_members
};

gd_nm_node_t
gd_app_runing_module_data_load(
    gd_app_context_t context,
    cpe_hash_string_t moduleName,
    cfg_t moduleCfg)
{
    gd_nm_mgr_t nmm;
    gd_nm_node_t rootGroup;
    gd_nm_node_t moduleGroup;

    nmm = gd_app_nm_mgr(context);

    rootGroup = gd_nm_mgr_find_node(nmm, gd_app_module_root_group_name);
    if (rootGroup == NULL) {
        rootGroup = gd_nm_group_create(nmm, cpe_hs_data(gd_app_module_root_group_name), 0);
        if (rootGroup == NULL) {
            APP_CTX_ERROR(context, "create module %s: data root group fail!", cpe_hs_data(moduleName));
            return NULL;
        }
        gd_nm_node_set_type(rootGroup, &g_module_root_group);
    }

    moduleGroup = gd_nm_group_create(gd_app_nm_mgr(context), cpe_hs_data(moduleName), 0);
    if (moduleGroup == NULL) {
        APP_CTX_ERROR(context, "create module %s: data group fail!", cpe_hs_data(moduleName));
        gd_app_runing_module_data_free(context, moduleName);
        return NULL;
    }

    gd_nm_node_set_type(moduleGroup, &g_module_group);

    if (gd_nm_group_add_member(rootGroup, moduleGroup) != 0) {
        APP_CTX_ERROR(context, "create module %s: add to root group fail!", cpe_hs_data(moduleName));
        gd_nm_node_free(moduleGroup);
        gd_app_runing_module_data_free(context, moduleName);
        return NULL;
    }

    return moduleGroup;
}

void gd_app_runing_module_data_free(gd_app_context_t context, cpe_hash_string_t moduleName) {
    gd_nm_mgr_t nmm;
    gd_nm_node_t rootGroup;
    gd_nm_node_t moduleGroup;

    nmm = gd_app_nm_mgr(context);

    rootGroup = gd_nm_mgr_find_node(nmm, gd_app_module_root_group_name);
    if (rootGroup == NULL) return;

    moduleGroup = gd_nm_group_find_member(rootGroup, moduleName);
    if (moduleGroup) {
        gd_nm_node_free(moduleGroup);
    }

    if (gd_nm_group_member_count(rootGroup) == 0) {
        gd_nm_node_free(rootGroup);
    }
}

gd_nm_node_t
gd_app_module_data(gd_app_context_t context, struct gd_app_module * module) {
    gd_nm_mgr_t nmm;
    gd_nm_node_t rootGroup;

    nmm = gd_app_nm_mgr(context);

    rootGroup = gd_nm_mgr_find_node(nmm, gd_app_module_root_group_name);

    return rootGroup ?
        gd_nm_group_find_member(rootGroup, module->m_name)
        : NULL;
}


CPE_HS_DEF_VAR(gd_app_module_root_group_name, "app_modules");
