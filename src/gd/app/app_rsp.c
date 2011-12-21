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

static int gd_app_rsp_create(
    gd_app_context_t context,
    gd_app_module_t module,
    gd_dp_mgr_t dpm,
    gd_nm_node_t rspNode,
    cfg_t cfgNode)
{
    const char * rspName;

    rspName = cfg_get_string(cfgNode, "name", NULL);
    if (rspName == NULL) {
        APP_CTX_ERROR(context, "%s reading rsp: no module node!", gd_app_module_name(module));
    }

    return 0;
}

static gd_nm_node_t
gd_app_rsp_create_group_node(gd_app_context_t context, gd_app_module_t module) {
    gd_nm_node_t moduleNode;
    gd_nm_node_t rspNode;

    moduleNode = gd_app_module_data(context, module);
    if (moduleNode == NULL) {
        APP_CTX_ERROR(context, "%s reading rsp: no module node!", gd_app_module_name(module));
        return NULL;
    }

    rspNode = gd_nm_group_create(gd_app_nm_mgr(context), "rsps", 0);
    if (rspNode == NULL) {
        APP_CTX_ERROR(context, "%s reading rsp: create rsps node fail!", gd_app_module_name(module));
        return NULL;
    }

    gd_nm_node_set_type(rspNode, &g_module_rsp_group);

    if (gd_nm_group_add_member(moduleNode, rspNode) != 0) {
        APP_CTX_ERROR(context, "create module %s: rsp node to module fail!", gd_app_module_name(module));
        gd_nm_node_free(rspNode);
        return NULL;
    }

    return rspNode;
}

int gd_app_rsp_load(
    gd_app_context_t context,
    gd_app_module_t module,
    cfg_t moduleCfg)
{
    int rv;
    cfg_t rsps;
    cfg_t rspCfg;
    gd_nm_node_t rspNode;
    struct cfg_seq_it cfgIt;
    gd_dp_mgr_t dpm;

    assert(context);
    assert(module);

    rsps = cfg_find_cfg(moduleCfg, "rsps");
    if (rsps == NULL) return 0;

    if (cfg_type(rsps) != CPE_CFG_TYPE_SEQUENCE) {
        APP_CTX_ERROR(
            context,
            "%s reading rsp: config node type error!",
            gd_app_module_name(module));
        return -1;
    }

    rspNode = gd_app_rsp_create_group_node(context, module);
    if (rspNode == NULL) return -1;

    dpm = gd_app_dp_mgr(context);

    rv = 0;
    cfg_seq_it_init(&cfgIt, rsps);
    while(rv == 0
          && (rspCfg = cfg_seq_it_next(&cfgIt)))
    {
        if (gd_app_rsp_create(context, module, dpm, rspNode, rspCfg) != 0) {
            rv = -1;
        }
    }

    return 0;
}

