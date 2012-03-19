#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_req.h"
#include "usf/logic/logic_manage.h"
#include "usf/dr_store/dr_ref.h"
#include "usf/dr_store/dr_store_manage.h"
#include "bpg_internal_ops.h"

static void bpg_manage_clear(gd_nm_node_t node);

static cpe_hash_string_buf s_bpg_manage_default_name = CPE_HS_BUF_MAKE("bpg_manage");

struct gd_nm_node_type s_nm_node_type_bpg_manage = {
    "usf_bpg_manage",
    bpg_manage_clear
};

bpg_manage_t
bpg_manage_create(
    gd_app_context_t app,
    const char * name,
    logic_manage_t logic_mgr,
    error_monitor_t em)
{
    bpg_manage_t mgr;
    gd_nm_node_t mgr_node;

    assert(app);

    if (logic_mgr == 0) logic_mgr = logic_manage_default(app);
    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_bpg_manage_default_name);
    if (em == 0) em = gd_app_em(app);

    if (logic_mgr == 0) {
        CPE_ERROR(em, "%s: create: logic_mgr not exist!", name);
        return NULL;
    }

    mgr_node = gd_nm_group_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_manage_t)gd_nm_node_data(mgr_node);

    mgr->m_app = app;
    mgr->m_alloc = gd_app_alloc(app);
    mgr->m_metalib_ref = NULL;
    mgr->m_logic_mgr = logic_mgr;
    mgr->m_em = em;
    mgr->m_flags = 0;

    mgr->m_request_meta_name[0] = 0;
    mgr->m_request_meta = NULL;

    mgr->m_response_meta_name[0] = 0;
    mgr->m_response_meta = NULL;

    mgr->m_ctx_capacity = 0;
    mgr->m_ctx_init = NULL;
    mgr->m_ctx_fini = NULL;
    mgr->m_ctx_ctx = NULL;

    mgr->m_rsp_max_size = 4 * 1024;
    mgr->m_rsp_buf = NULL;

    mgr->m_debug = 0;

    mgr->m_send_to = NULL;

    mgr->m_metalib_basepkg_ref =
        dr_ref_create(
            dr_store_manage_default(mgr->m_app),
            BPG_BASEPKG_NAME);
    if (mgr->m_metalib_basepkg_ref == NULL) {
        CPE_ERROR(em, "%s: create: create basepkg_ref fail!", name);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    mgr->m_running_req_sn = 0;
    if (cpe_hash_table_init(
            &mgr->m_running_reqs,
            mgr->m_alloc,
            (cpe_hash_fun_t) bpg_running_req_hash,
            (cpe_hash_cmp_t) bpg_running_req_cmp,
            CPE_HASH_OBJ2ENTRY(bpg_running_req, m_hh),
            -1) != 0)
    {
        dr_ref_free(mgr->m_metalib_basepkg_ref);
        gd_nm_node_free(mgr_node);
        return NULL;
    }

    gd_nm_node_set_type(mgr_node, &s_nm_node_type_bpg_manage);

    return mgr;
}

static void bpg_manage_clear(gd_nm_node_t node) {
    bpg_manage_t mgr;
    mgr = (bpg_manage_t)gd_nm_node_data(node);

    bpg_running_req_free_all(mgr);
    cpe_hash_table_fini(&mgr->m_running_reqs);

    if (mgr->m_metalib_basepkg_ref) {
        dr_ref_free(mgr->m_metalib_basepkg_ref);
        mgr->m_metalib_ref = NULL;
    }

    if (mgr->m_metalib_ref) {
        dr_ref_free(mgr->m_metalib_ref);
        mgr->m_metalib_ref = NULL;
    }

    if (mgr->m_rsp_buf) {
        bpg_req_free(mgr->m_rsp_buf);
        mgr->m_rsp_buf = NULL;
    }

    if (mgr->m_send_to) {
        mem_free(mgr->m_alloc, mgr->m_send_to);
        mgr->m_send_to = NULL;
    }
}

void bpg_manage_free(bpg_manage_t mgr) {
    gd_nm_node_t mgr_node;
    assert(mgr);

    mgr_node = gd_nm_node_from_data(mgr);
    if (gd_nm_node_type(mgr_node) != &s_nm_node_type_bpg_manage) return;
    gd_nm_node_free(mgr_node);
}

bpg_manage_t
bpg_manage_find(gd_app_context_t app, cpe_hash_string_t name) {
    gd_nm_node_t node;

    if (name == NULL) name = (cpe_hash_string_t)&s_bpg_manage_default_name;

    node = gd_nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_manage) return NULL;
    return (bpg_manage_t)gd_nm_node_data(node);
}

bpg_manage_t
bpg_manage_find_nc(gd_app_context_t app, const char * name) {
    gd_nm_node_t node;

    if (name == NULL) return bpg_manage_default(app);

    node = gd_nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_bpg_manage) return NULL;
    return (bpg_manage_t)gd_nm_node_data(node);
}

bpg_manage_t
bpg_manage_default(gd_app_context_t app) {
    return bpg_manage_find(app, (cpe_hash_string_t)&s_bpg_manage_default_name);
}

gd_app_context_t bpg_manage_app(bpg_manage_t mgr) {
    return mgr->m_app;
}

const char * bpg_manage_name(bpg_manage_t mgr) {
    return gd_nm_node_name(gd_nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_manage_name_hs(bpg_manage_t mgr) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(mgr));
}

const char * bpg_manage_send_to(bpg_manage_t mgr) {
    return mgr->m_send_to ? cpe_hs_data(mgr->m_send_to) : NULL;
}

int bpg_manage_set_send_to(bpg_manage_t mgr, const char * name) {
    cpe_hash_string_t new_send_to;

    new_send_to = cpe_hs_create(mgr->m_alloc, name);
    if (new_send_to == NULL) return -1;

    if (mgr->m_send_to) mem_free(mgr->m_alloc, mgr->m_send_to);
    mgr->m_send_to = new_send_to;

    return 0;
}

bpg_req_t
bpg_manage_rsp_buf(bpg_manage_t mgr, LPDRMETA carry_meta, size_t caary_capacity) {
    if (mgr->m_rsp_buf) {
        if (bpg_req_carry_data_meta(mgr->m_rsp_buf) != carry_meta
            || bpg_req_carry_data_capacity(mgr->m_rsp_buf) < caary_capacity
            || bpg_req_pkg_capacity(mgr->m_rsp_buf) < mgr->m_rsp_max_size)
        {
            bpg_req_free(mgr->m_rsp_buf);
            mgr->m_rsp_buf = NULL;
        }
    }

    if (mgr->m_rsp_buf == NULL) {
        mgr->m_rsp_buf = bpg_req_create(mgr->m_app, mgr->m_rsp_max_size, carry_meta, caary_capacity);
    }

    return mgr->m_rsp_buf;
}

LPDRMETA bpg_manage_request_meta(bpg_manage_t mgr) {
    if (mgr->m_request_meta == NULL && mgr->m_request_meta_name[0]) {
        LPDRMETALIB metalib = bpg_manage_metalib(mgr);
        if (metalib) {
            mgr->m_request_meta = dr_lib_find_meta_by_name(metalib, mgr->m_request_meta_name);
        }
    }

    return mgr->m_request_meta;
}

const char *
bpg_manage_request_meta_name(bpg_manage_t mgr) {
    return mgr->m_request_meta_name;
}

int bpg_manage_set_request_meta_name(bpg_manage_t mgr, const char * name) {
    size_t name_len;

    name_len = strlen(name) + 1;
    if (name_len > sizeof(mgr->m_request_meta_name)) {
        CPE_ERROR(
            mgr->m_em, "bpg_manage %s: set request meta name %s, name len overflow!", 
            bpg_manage_name(mgr), name);
        return -1;
    }

    memcpy(mgr->m_request_meta_name, name, name_len);
    mgr->m_request_meta = NULL;

    return 0;
}

LPDRMETA bpg_manage_response_meta(bpg_manage_t mgr) {
    if (mgr->m_response_meta == NULL && mgr->m_response_meta_name[0]) {
        LPDRMETALIB metalib = bpg_manage_metalib(mgr);
        if (metalib) {
            mgr->m_response_meta = dr_lib_find_meta_by_name(metalib, mgr->m_response_meta_name);
        }
    }

    return mgr->m_response_meta;
}

const char * bpg_manage_response_meta_name(bpg_manage_t mgr) {
    return mgr->m_response_meta_name;
}

int bpg_manage_set_response_meta_name(bpg_manage_t mgr, const char * name) {
    size_t name_len;

    name_len = strlen(name) + 1;
    if (name_len > sizeof(mgr->m_response_meta_name)) {
        CPE_ERROR(
            mgr->m_em, "bpg_manage %s: set response meta name %s, name len overflow!", 
            bpg_manage_name(mgr), name);
        return -1;
    }

    memcpy(mgr->m_response_meta_name, name, name_len);
    mgr->m_response_meta = NULL;

    return 0;
}

const char * bpg_manage_metalib_name(bpg_manage_t mgr) {
    return mgr->m_metalib_ref ? dr_ref_lib_name(mgr->m_metalib_ref) : "???";
}

LPDRMETALIB bpg_manage_metalib(bpg_manage_t mgr) {
    return mgr->m_metalib_ref ? dr_ref_lib(mgr->m_metalib_ref) : NULL;
}

int bpg_manage_set_metalib(bpg_manage_t mgr, const char * metalib_name) {
    assert(mgr);
    assert(metalib_name);

    if (mgr->m_metalib_ref) dr_ref_free(mgr->m_metalib_ref);

    mgr->m_metalib_ref =
        dr_ref_create(
            dr_store_manage_default(mgr->m_app),
            metalib_name);
    if (mgr->m_metalib_ref == NULL) {
        CPE_ERROR(
            mgr->m_em, "bpg_manage %s: set metalib %s, create dr_ref fail!", 
            bpg_manage_name(mgr), metalib_name);
        return -1;
    }

    mgr->m_request_meta = NULL;
    mgr->m_response_meta = NULL;

    return 0;
}

uint32_t bpg_manage_flags(bpg_manage_t mgr) {
    return mgr->m_flags;
}

void bpg_manage_flags_set(bpg_manage_t mgr, uint32_t flag) {
    mgr->m_flags = flag;
}

void bpg_manage_flag_enable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    mgr->m_flags |= flag;
}

void bpg_manage_flag_disable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    mgr->m_flags &= ~((uint32_t)flag);
}

int bpg_manage_flag_is_enable(bpg_manage_t mgr, bpg_manage_flag_t flag) {
    return mgr->m_flags & flag;
}

void bpg_manage_set_context_op(
    bpg_manage_t mgr,
    size_t ctx_capacity,
    bpg_logic_ctx_init_fun_t ctx_init,
    bpg_logic_ctx_fini_fun_t ctx_fini,
    void * ctx_ctx)
{
    mgr->m_ctx_capacity = ctx_capacity;
    mgr->m_ctx_init = ctx_init;
    mgr->m_ctx_fini = ctx_fini;
    mgr->m_ctx_ctx = ctx_ctx;
}

LPDRMETALIB bpg_metalib(bpg_manage_t mgr) {
    return mgr->m_metalib_basepkg_ref ? dr_ref_lib(mgr->m_metalib_basepkg_ref) : NULL;
}

LPDRMETA bpg_meta_pkghead(bpg_manage_t mgr) {
    return dr_lib_find_meta_by_name(bpg_metalib(mgr), "basepkg_head");
}

LPDRMETA bpg_meta_pkg(bpg_manage_t mgr) {
    return dr_lib_find_meta_by_name(bpg_metalib(mgr), "basepkg");
}

