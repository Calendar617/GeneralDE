#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/app/app_library.h"
#include "usf/dr_store/dr_store.h"
#include "usf/dr_store/dr_store_manage.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_rsp.h"
#include "bpg_internal_types.h"

static int bpg_manage_app_validate_meta(gd_app_context_t app, gd_app_module_t module, bpg_manage_t mgr) {
    if (bpg_manage_metalib(mgr) == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: validate meta info: meta lib %s not exist!",
            bpg_manage_name(mgr), bpg_manage_metalib_name(mgr));
        return -1;
    }

    if (bpg_manage_request_meta(mgr) == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: validate meta info: request meta %s not exist in lib %s!",
            bpg_manage_name(mgr), bpg_manage_request_meta_name(mgr), bpg_manage_metalib_name(mgr));
        return -1;
    }

    if (bpg_manage_response_meta(mgr) == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: validate meta info: response meta %s not exist in lib %s!",
            bpg_manage_name(mgr), bpg_manage_response_meta_name(mgr), bpg_manage_metalib_name(mgr));
        return -1;
    }

    return 0;
}

static int bpg_manage_app_load_meta(gd_app_context_t app, gd_app_module_t module, bpg_manage_t mgr, cfg_t cfg) {
    const char * arg;
    
    if ((arg = cfg_get_string(cfg, "lib-name", NULL))) {
        if (bpg_manage_set_metalib(mgr, arg) != 0) {
            CPE_ERROR(
                gd_app_em(app), "%s: load meta info: set meta ref %s fail!",
                bpg_manage_name(mgr), arg);
            return -1;
        }
    }
    else {
        CPE_ERROR(
            gd_app_em(app), "%s: load meta info: lib-name not configured!",
            bpg_manage_name(mgr));
        return -1;
    }

    if ((arg = cfg_get_string(cfg, "req-type-name", NULL))) {
        if (bpg_manage_set_request_meta_name(mgr, arg) != 0) {
            CPE_ERROR(
                gd_app_em(app), "%s: load meta info: set request-meta-name %s fail!",
                gd_app_module_name(module), arg);
            return -1;
        }
    }
    else {
        CPE_ERROR(
            gd_app_em(app), "%s: load meta info: req-type-name not configured!",
            gd_app_module_name(module));
        return -1;
    }

    if ((arg = cfg_get_string(cfg, "rsp-type-name", NULL))) {
        if (bpg_manage_set_response_meta_name(mgr, arg) != 0) {
            CPE_ERROR(
                gd_app_em(app), "%s: load meta info: set response-meta-name %s fail!",
                gd_app_module_name(module), arg);
            return -1;
        }
    }
    else {
        CPE_ERROR(
            gd_app_em(app), "%s: load meta info: meta.rsp-type-name not configured!",
            gd_app_module_name(module));
        return -1;
    }
        
    return 0;
}

static int bpg_manage_app_load_base_meta(gd_app_context_t app, gd_app_module_t module, int debug) {
    dr_store_manage_t dr_store_mgr;
    dr_store_t dr_store;
    LPDRMETALIB metalib;

    dr_store_mgr = dr_store_manage_default(app);
    if (dr_store_mgr == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: load base meta info: dr_store_manage_default not exist!",
            gd_app_module_name(module));
        return -1;
    }

    dr_store = dr_store_find_or_create(dr_store_mgr, BPG_BASEPKG_NAME);
    if (dr_store == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: load base meta info: dr_store %s create fail!",
            gd_app_module_name(module), BPG_BASEPKG_NAME);
        return -1;
    }

    if (dr_store_lib(dr_store)) {
        if (debug) {
            CPE_ERROR(
                gd_app_em(app), "%s: load base meta info: use extern metalib",
                gd_app_module_name(module));
        }

        return 0;
    }

    metalib = (LPDRMETALIB)gd_app_lib_sym(
        gd_app_module_lib(module), "g_metalib_base_package", gd_app_em(app));
    if (metalib == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: find default metalib symbol fail!",
            gd_app_module_name(module));
        return -1;
    }

    if (dr_store_set_lib(dr_store, metalib, NULL, NULL) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: set default metalib fail!",
            gd_app_module_name(module));
        return -1;
    }

    if (debug) {
        CPE_ERROR(
            gd_app_em(app), "%s: load base meta info: use self metalib",
            gd_app_module_name(module));
    }

    return 1;
}

static void bpg_manage_app_unload_base_meta(gd_app_context_t app, gd_app_module_t module) {
    dr_store_manage_t dr_store_mgr;
    dr_store_t dr_store;
    LPDRMETALIB metalib;

    dr_store_mgr = dr_store_manage_default(app);
    if (dr_store_mgr == NULL) return;

    dr_store = dr_store_find_or_create(dr_store_mgr, BPG_BASEPKG_NAME);
    if (dr_store == NULL) return;

    if (dr_store_lib(dr_store) == NULL) return;

    metalib = (LPDRMETALIB)gd_app_lib_sym(
        gd_app_module_lib(module), "g_metalib_base_package", gd_app_em(app));
    if (metalib == NULL) return;

    if (dr_store_lib(dr_store) == metalib) {
        dr_store_reset_lib(dr_store, NULL, NULL, NULL);
    }
}

EXPORT_DIRECTIVE
int bpg_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_manage_t bpg_manage;
    cfg_t child_cfg;
    int basepkg_lib_state;
    int debug;

    debug = cfg_get_int32(cfg, "debug", 0);

    basepkg_lib_state = bpg_manage_app_load_base_meta(app, module, debug);
    if (basepkg_lib_state < 0) return -1;

    bpg_manage = bpg_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_manage == NULL) {
        bpg_manage_app_unload_base_meta(app, module);
        return -1;
    }

    child_cfg = cfg_find_cfg(cfg, "meta");
    if (child_cfg) {
        if (bpg_manage_app_load_meta(app, module, bpg_manage, child_cfg) != 0
            || (
                cfg_get_int32(child_cfg, "validate", 0) != 0
                && bpg_manage_app_validate_meta(app, module, bpg_manage) != 0))
        {
            bpg_manage_app_unload_base_meta(app, module);
            bpg_manage_free(bpg_manage);
            return -1;
        }
    }

    child_cfg = cfg_find_cfg(cfg, "rsps");
    if (child_cfg) {
        if (bpg_rsp_build(bpg_manage, child_cfg, gd_app_em(app)) != 0) {
            bpg_manage_app_unload_base_meta(app, module);
            bpg_manage_free(bpg_manage);
            return -1;
        }
    }

    bpg_manage->m_rsp_max_size =
        cfg_get_uint32(cfg, "rsp-max-size", bpg_manage->m_rsp_max_size);

    bpg_manage->m_debug = debug;

    if (bpg_manage->m_debug) {
        CPE_INFO(
            gd_app_em(app),
            "%s: create: done. rsp-max-size=%d",
            gd_app_module_name(module), (int)bpg_manage->m_rsp_max_size);
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_manage_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_manage_t bpg_manage;

    bpg_manage = bpg_manage_find_nc(app, gd_app_module_name(module));
    if (bpg_manage) {
        bpg_manage_free(bpg_manage);
    }

    bpg_manage_app_unload_base_meta(app, module);
}
