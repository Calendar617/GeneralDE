#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/app/app_library.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_rsp.h"

static int bpg_manager_app_load_metalib(bpg_manage_t mgr, cfg_t cfg, error_monitor_t em) {
    const char * metalib_name;
    LPDRMETALIB metalib;
    
    metalib_name = cfg_get_string(cfg, "metalib-name", NULL);
    if (metalib_name == NULL) return 0;

    metalib = (LPDRMETALIB)gd_app_lib_sym(NULL, metalib_name, em);
    if (metalib == NULL) {
        CPE_ERROR(
            em, "bpg_manager %s: create: load metalib %s fail!",
            bpg_manage_name(mgr), metalib_name);
        return -1;
    }

    if (bpg_manage_set_metalib(mgr, metalib) != 0) {
        CPE_ERROR(
            em, "bpg_manager %s: create: set metalib %s fail!",
            bpg_manage_name(mgr), metalib_name);
        return -1;
    }

    return 0;
}

EXPORT_DIRECTIVE
int bpg_manager_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_manage_t bpg_manager;
    const char * request_meta_name;
    const char * response_meta_name;
    cfg_t rsp_cfg;

    request_meta_name = cfg_get_string(cfg, "request-meta-name", NULL);
    if (request_meta_name == NULL) {
        CPE_ERROR(
            gd_app_em(app), "bpg_manager %s: create: request-meta-name not configured!",
            gd_app_module_name(module));
        return -1;
    }
    response_meta_name = cfg_get_string(cfg, "response-meta-name", request_meta_name);

    bpg_manager = bpg_manage_create(app, gd_app_module_name(module), NULL, NULL);
    if (bpg_manager == NULL) return -1;

    if (bpg_manager_app_load_metalib(bpg_manager, cfg, gd_app_em(app)) != 0) {
        bpg_manage_free(bpg_manager);
        return -1;
    }

    if (bpg_manage_set_response_meta_name(bpg_manager, response_meta_name) != 0) {
        CPE_ERROR(
            gd_app_em(app), "bpg_manager %s: create: set response-meta-name %s fail!",
            gd_app_module_name(module), response_meta_name);
        bpg_manage_free(bpg_manager);
        return -1;
    }
        
    if (bpg_manage_set_request_meta_name(bpg_manager, request_meta_name) != 0) {
        CPE_ERROR(
            gd_app_em(app), "bpg_manager %s: create: set request-meta-name %s fail!",
            gd_app_module_name(module), request_meta_name);
        bpg_manage_free(bpg_manager);
        return -1;
    }
        
    rsp_cfg = cfg_find_cfg(cfg, "rsps");
    if (rsp_cfg) {
        if (bpg_rsp_build(bpg_manager, rsp_cfg, gd_app_em(app)) != 0) {
            bpg_manage_free(bpg_manager);
            return -1;
        }
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_manager_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_manage_t bpg_manager;

    bpg_manager = bpg_manage_find_nc(app, gd_app_module_name(module));
    if (bpg_manager) {
        bpg_manage_free(bpg_manager);
    }
}

