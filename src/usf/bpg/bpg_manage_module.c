#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
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

EXPORT_DIRECTIVE
int bpg_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_manage_t bpg_manage;
    cfg_t child_cfg;

    bpg_manage = bpg_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_manage == NULL) return -1;

    child_cfg = cfg_find_cfg(cfg, "meta");
    if (child_cfg) {
        if (bpg_manage_app_load_meta(app, module, bpg_manage, child_cfg) != 0
            || (
                cfg_get_int32(child_cfg, "validate", 0) != 0
                && bpg_manage_app_validate_meta(app, module, bpg_manage) != 0))
        {
            bpg_manage_free(bpg_manage);
            return -1;
        }
    }

    child_cfg = cfg_find_cfg(cfg, "rsps");
    if (child_cfg) {
        if (bpg_rsp_build(bpg_manage, child_cfg, gd_app_em(app)) != 0) {
            bpg_manage_free(bpg_manage);
            return -1;
        }
    }

    bpg_manage->m_rsp_max_size =
        cfg_get_uint32(cfg, "rsp-max-size", bpg_manage->m_rsp_max_size);

    bpg_manage->m_debug = cfg_get_int32(cfg, "debug", 0);

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
}
