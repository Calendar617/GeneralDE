#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_rsp.h"
#include "bpg_internal_types.h"

static int bpg_manage_app_load_metalib(bpg_manage_t mgr, cfg_t cfg, error_monitor_t em) {
    const char * metalib_name;
    
    metalib_name = cfg_get_string(cfg, "meta.lib-name", NULL);
    if (metalib_name == NULL) return 0;

    if (bpg_manage_set_metalib(mgr, metalib_name) != 0) {
        CPE_ERROR(
            em, "%s: create: set metalib %s fail!",
            bpg_manage_name(mgr), metalib_name);
        return -1;
    }

    return 0;
}

EXPORT_DIRECTIVE
int bpg_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_manage_t bpg_manage;
    const char * request_meta_name;
    const char * response_meta_name;
    cfg_t rsp_cfg;

    request_meta_name = cfg_get_string(cfg, "meta.req-type-name", NULL);
    if (request_meta_name == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: meta.req-type-name not configured!",
            gd_app_module_name(module));
        return -1;
    }
    response_meta_name = cfg_get_string(cfg, "meta.rsp-type-name", NULL);
    if (response_meta_name == NULL) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: meta.rsp-type-name not configured!",
            gd_app_module_name(module));
        return -1;
    }

    bpg_manage = bpg_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_manage == NULL) return -1;

    if (bpg_manage_app_load_metalib(bpg_manage, cfg, gd_app_em(app)) != 0) {
        bpg_manage_free(bpg_manage);
        return -1;
    }

    if (bpg_manage_set_response_meta_name(bpg_manage, response_meta_name) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set response-meta-name %s fail!",
            gd_app_module_name(module), response_meta_name);
        bpg_manage_free(bpg_manage);
        return -1;
    }
        
    if (bpg_manage_set_request_meta_name(bpg_manage, request_meta_name) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set request-meta-name %s fail!",
            gd_app_module_name(module), request_meta_name);
        bpg_manage_free(bpg_manage);
        return -1;
    }
        
    rsp_cfg = cfg_find_cfg(cfg, "rsps");
    if (rsp_cfg) {
        if (bpg_rsp_build(bpg_manage, rsp_cfg, gd_app_em(app)) != 0) {
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
