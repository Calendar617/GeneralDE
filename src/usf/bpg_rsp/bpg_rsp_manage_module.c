#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/app/app_library.h"
#include "usf/dr_store/dr_store.h"
#include "usf/dr_store/dr_store_manage.h"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "usf/bpg_rsp/bpg_rsp.h"
#include "bpg_rsp_internal_types.h"

EXPORT_DIRECTIVE
int bpg_rsp_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_rsp_manage_t bpg_rsp_manage;
    cfg_t child_cfg;
    int debug;

    debug = cfg_get_int32(cfg, "debug", 0);

    bpg_rsp_manage = bpg_rsp_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_rsp_manage == NULL) {
        return -1;
    }

    child_cfg = cfg_find_cfg(cfg, "rsps");
    if (child_cfg) {
        if (bpg_rsp_build(bpg_rsp_manage, child_cfg, gd_app_em(app)) != 0) {
            bpg_rsp_manage_free(bpg_rsp_manage);
            return -1;
        }
    }

    bpg_rsp_manage->m_rsp_max_size =
        cfg_get_uint32(cfg, "rsp-max-size", bpg_rsp_manage->m_rsp_max_size);

    bpg_rsp_manage->m_debug = debug;

    if (bpg_rsp_manage->m_debug) {
        CPE_INFO(
            gd_app_em(app),
            "%s: create: done. rsp-max-size=%d",
            gd_app_module_name(module), (int)bpg_rsp_manage->m_rsp_max_size);
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_rsp_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_rsp_manage_t bpg_rsp_manage;

    bpg_rsp_manage = bpg_rsp_manage_find_nc(app, gd_app_module_name(module));
    if (bpg_rsp_manage) {
        bpg_rsp_manage_free(bpg_rsp_manage);
    }
}
