#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_module.h"
#include "gd/app/app_context.h"
#include "usf/dr_store/dr_ref.h"
#include "usf/dr_store/dr_store_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "bpg_pkg_internal_types.h"

static void bpg_pkg_manage_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_bpg_pkg_manage_default_name = CPE_HS_BUF_MAKE("bpg_pkg_manage");

struct gd_nm_node_type s_nm_node_type_bpg_pkg_manage = {
    "usf_bpg_pkg_manage",
    bpg_pkg_manage_clear
};

bpg_pkg_manage_t
bpg_pkg_manage_create(
    gd_app_context_t app,
    const char * name,
    error_monitor_t em)
{
    bpg_pkg_manage_t mgr;
    gd_nm_node_t mgr_node;

    assert(app);

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_bpg_pkg_manage_default_name);
    if (em == 0) em = gd_app_em(app);

    mgr_node = gd_nm_group_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_pkg_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_pkg_manage_t)gd_nm_node_data(mgr_node);

    mgr->m_app = app;
    mgr->m_alloc = gd_app_alloc(app);
    mgr->m_em = em;
    mgr->m_debug = 0;
    mgr->m_base_cvt = NULL;
    mgr->m_data_cvt = NULL;

    mgr->m_metalib_basepkg_ref =
        dr_ref_create(
            dr_store_manage_default(mgr->m_app),
            BPG_BASEPKG_LIB_NAME);
    if (mgr->m_metalib_basepkg_ref == NULL) {
        CPE_ERROR(em, "%s: create: create basepkg_ref fail!", name);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_bpg_pkg_manage);

    return mgr;
}

static void bpg_pkg_manage_clear(gd_nm_node_t node) {
    bpg_pkg_manage_t mgr;
    mgr = (bpg_pkg_manage_t)gd_nm_node_data(node);

    if (mgr->m_data_cvt) {
        dr_cvt_free(mgr->m_data_cvt);
        mgr->m_data_cvt = NULL;
    }

    if (mgr->m_base_cvt) {
        dr_cvt_free(mgr->m_base_cvt);
        mgr->m_base_cvt = NULL;
    }

    if (mgr->m_metalib_basepkg_ref) {
        dr_ref_free(mgr->m_metalib_basepkg_ref);
        mgr->m_metalib_basepkg_ref = NULL;
    }
}

void bpg_pkg_manage_free(bpg_pkg_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_bpg_pkg_manage) return;
    gd_nm_node_free(mgr_node);
}

bpg_pkg_manage_t
bpg_pkg_manage_find(gd_app_context_t app, cpe_hash_string_t name) {
    gd_nm_node_t node;

    if (name == NULL) name = (cpe_hash_string_t)&s_bpg_pkg_manage_default_name;

    node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_pkg_manage) return NULL;
    return (bpg_pkg_manage_t)gd_nm_node_data(node);
}

bpg_pkg_manage_t
bpg_pkg_manage_find_nc(gd_app_context_t app, const char * name) {
    gd_nm_node_t node;

    if (name == NULL) return bpg_pkg_manage_default(app);

    node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_pkg_manage) return NULL;
    return (bpg_pkg_manage_t)gd_nm_node_data(node);
}

bpg_pkg_manage_t
bpg_pkg_manage_default(gd_app_context_t app) {
    return bpg_pkg_manage_find(app, (cpe_hash_string_t)&s_bpg_pkg_manage_default_name);
}

gd_app_context_t bpg_pkg_manage_app(bpg_pkg_manage_t mgr) {
    return mgr->m_app;
}

const char * bpg_pkg_manage_name(bpg_pkg_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_pkg_manage_name_hs(bpg_pkg_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}


int bpg_pkg_manage_set_base_cvt(bpg_pkg_manage_t mgr, const char * cvt) {
    dr_cvt_t new_cvt;

    if (mgr->m_base_cvt && strcmp(dr_cvt_name(mgr->m_base_cvt), cvt) == 0) return 0;

    new_cvt = dr_cvt_create(cvt);
    if (new_cvt == NULL) return -1;

    if (mgr->m_base_cvt) dr_cvt_free(mgr->m_base_cvt);

    mgr->m_base_cvt = new_cvt;
    return 0;
}


dr_cvt_t bpg_pkg_manage_base_cvt(bpg_pkg_manage_t mgr) {
    return mgr->m_base_cvt;
}

const char * bpg_pkg_manage_base_cvt_name(bpg_pkg_manage_t mgr) {
    return mgr->m_base_cvt ? dr_cvt_name(mgr->m_base_cvt) : "";
}

int bpg_pkg_manage_set_data_cvt(bpg_pkg_manage_t mgr, const char * cvt) {
    dr_cvt_t new_cvt;

    if (mgr->m_data_cvt && strcmp(dr_cvt_name(mgr->m_data_cvt), cvt) == 0) return 0;

    new_cvt = dr_cvt_create(cvt);
    if (new_cvt == NULL) return -1;

    if (mgr->m_data_cvt) dr_cvt_free(mgr->m_data_cvt);

    mgr->m_data_cvt = new_cvt;
    return 0;
}

dr_cvt_t bpg_pkg_manage_data_cvt(bpg_pkg_manage_t mgr) {
    return mgr->m_data_cvt;
}

const char * bpg_pkg_manage_data_cvt_name(bpg_pkg_manage_t mgr) {
    return mgr->m_data_cvt ? dr_cvt_name(mgr->m_data_cvt) : "";
}

EXPORT_DIRECTIVE
int bpg_pkg_manage_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_pkg_manage_t bpg_pkg_manage;

    bpg_pkg_manage = bpg_pkg_manage_create(app, gd_app_module_name(module), NULL);
    if (bpg_pkg_manage == NULL) {
        return -1;
    }

    if (bpg_pkg_manage_set_base_cvt(bpg_pkg_manage, cfg_get_string(cfg, "base-cvt", NULL)) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set base-cvt %s f1il",
            gd_app_module_name(module), cfg_get_string(cfg, "base-cvt", NULL));
        bpg_pkg_manage_free(bpg_pkg_manage);
        return -1;
    }

    if (bpg_pkg_manage_set_data_cvt(bpg_pkg_manage, cfg_get_string(cfg, "data-cvt", NULL)) != 0) {
        CPE_ERROR(
            gd_app_em(app), "%s: create: set data-cvt %s f1il",
            gd_app_module_name(module), cfg_get_string(cfg, "data-cvt", NULL));
        bpg_pkg_manage_free(bpg_pkg_manage);
        return -1;
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
