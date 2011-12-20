#include <assert.h>
#include <string.h>
#include "cpe/cfg/cfg_read.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app.h"
#include "gd/app/app_context.h"
#include "gd/app/app_rsp.h"
#include "app_internal_ops.h"

static struct gd_nm_node_type g_module_rsp_group = {
    "app_module_rsp",
    gd_nm_group_free_members
};

int gd_app_load_rsps(
    gd_app_context_t context,
    gd_app_module_t module,
    cfg_t moduleCfg)
{
    cfg_t rsps;
    gd_nm_node_t moduleNode;
    gd_nm_node_t rspNode;

    rsps = cfg_find_cfg(moduleCfg, "rsp");
    if (rsps == NULL) return 0;

    if (cfg_type(rsps) != CPE_CFG_TYPE_SEQUENCE) {
        APP_CTX_ERROR(context, "%s reading rsp: config node type error!", gd_app_module_name(module));
        return -1;
    }

    moduleNode = gd_app_module_data(context, module);
    if (moduleNode == NULL) {
        APP_CTX_ERROR(context, "%s reading rsp: no module node!", gd_app_module_name(module));
        return -1;
    }

    rspNode = gd_nm_group_create(gd_app_nm_mgr(context), "rsps", 0);
    if (rspNode == NULL) {
        APP_CTX_ERROR(context, "%s reading rsp: create rsps node fail!", gd_app_module_name(module));
        return -1;
    }

    gd_nm_node_set_type(rspNode, &g_module_rsp_group);

    if (gd_nm_group_add_member(moduleNode, rspNode) != 0) {
        APP_CTX_ERROR(context, "create module %s: rsp node to module fail!", gd_app_module_name(module));
        gd_nm_node_free(rspNode);
        return -1;
    }

    
    return 0;
}

