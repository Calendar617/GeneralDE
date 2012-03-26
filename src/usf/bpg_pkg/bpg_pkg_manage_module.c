#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "gd/app/app_module.h"
#include "gd/app/app_context.h"
#include "gd/dr_store/dr_ref.h"
#include "gd/dr_store/dr_store_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "bpg_pkg_internal_types.h"

static int bpg_pkg_manage_app_validate_meta(gd_app_context_t app, gd_app_module_t module, bpg_pkg_manage_t mgr) {
    if (bpg_pkg_manage_data_metalib(mgr) == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: validate meta info: meta lib %s not exist!",
            bpg_pkg_manage_name(mgr), bpg_pkg_manage_data_metalib_name(mgr));
        return -1;
    }

    if (bpg_pkg_manage_cmd_meta(mgr) == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: validate meta info: request meta %s not exist in lib %s!",
            bpg_pkg_manage_name(mgr), bpg_pkg_manage_cmd_meta_name(mgr), bpg_pkg_manage_data_metalib_name(mgr));
        return -1;
    }

    return 0;
}

static int bpg_pkg_manage_app_load_meta(gd_app_context_t app, gd_app_module_t module, bpg_pkg_manage_t mgr, cfg_t cfg) {
    const char * arg;
    
    if ((arg = cfg_get_string(cfg, "lib-name", NULL))) {
        if (bpg_pkg_manage_set_data_metalib(mgr, arg) != 0) {
            CPE_ERROR(
                gd_app_em(app), "%s: load meta info: set meta ref %s fail!",
                bpg_pkg_manage_name(mgr), arg);
            return -1;
        }
    }
    else {
        CPE_ERROR(
            gd_app_em(app), "%s: load meta info: lib-name not configured!",
            bpg_pkg_manage_name(mgr));
        return -1;
    }

    if ((arg = cfg_get_string(cfg, "cmd-meta-name", NULL))) {
        if (bpg_pkg_manage_set_cmd_meta_name(mgr, arg) != 0) {
            CPE_ERROR(
                gd_app_em(app), "%s: load meta info: set cmd-meta-name %s fail!",
                gd_app_module_name(module), arg);
            return -1;
        }
    }
    else {
        CPE_ERROR(
            gd_app_em(app), "%s: load meta info: cmd-meta-name not configured!",
            gd_app_module_name(module));
        return -1;
    }

    return 0;
}

EXPORT_DIRECTIVE
int bpg_pkg_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_pkg_manage_t bpg_pkg_manage;
    cfg_t child_cfg;

    bpg_pkg_manage = bpg_pkg_manage_create(app, gd_app_module_name(module), NULL);
    if (bpg_pkg_manage == NULL) {
        return -1;
    }

    if (bpg_pkg_manage_set_base_cvt(bpg_pkg_manage, cfg_get_string(cfg, "base-cvt", NULL)) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set base-cvt %s fail",
            gd_app_module_name(module), cfg_get_string(cfg, "base-cvt", NULL));
        bpg_pkg_manage_free(bpg_pkg_manage);
        return -1;
    }

    if (bpg_pkg_manage_set_data_cvt(bpg_pkg_manage, cfg_get_string(cfg, "data-cvt", NULL)) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set data-cvt %s fail",
            gd_app_module_name(module), cfg_get_string(cfg, "data-cvt", NULL));
        bpg_pkg_manage_free(bpg_pkg_manage);
        return -1;
    }

    child_cfg = cfg_find_cfg(cfg, "meta");
    if (child_cfg) {
        if (bpg_pkg_manage_app_load_meta(app, module, bpg_pkg_manage, child_cfg) != 0
            || (
                cfg_get_int32(child_cfg, "validate", 0) != 0
                && bpg_pkg_manage_app_validate_meta(app, module, bpg_pkg_manage) != 0))
        {
            bpg_pkg_manage_free(bpg_pkg_manage);
            return -1;
        }
    }

    bpg_pkg_manage->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (bpg_pkg_manage->m_debug) {
        CPE_INFO(
            gd_app_em(app), "%s: create: done. base-cvt=%s, data-cvt=%s",
            gd_app_module_name(module),
            bpg_pkg_manage_base_cvt_name(bpg_pkg_manage),
            bpg_pkg_manage_data_cvt_name(bpg_pkg_manage));
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_pkg_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_pkg_manage_t bpg_pkg_manage;

    bpg_pkg_manage = bpg_pkg_manage_find_nc(app, gd_app_module_name(module));
    if (bpg_pkg_manage) {
        bpg_pkg_manage_free(bpg_pkg_manage);
    }
}
