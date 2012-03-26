#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "usf/logic/logic_manage.h"
#include "usf/dr_store/dr_ref.h"
#include "usf/dr_store/dr_store_manage.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_dsp.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "usf/bpg_rsp/bpg_rsp_pkg_builder.h"
#include "bpg_rsp_internal_ops.h"

static void bpg_rsp_manage_clear(nm_node_t node);

static cpe_hash_string_buf s_bpg_rsp_manage_default_name = CPE_HS_BUF_MAKE("bpg_rsp_manage");

struct nm_node_type s_nm_node_type_bpg_rsp_manage = {
    "usf_bpg_rsp_manage",
    bpg_rsp_manage_clear
};

bpg_rsp_manage_t
bpg_rsp_manage_create(
    gd_app_context_t app,
    const char * name,
    logic_manage_t logic_mgr,
    error_monitor_t em)
{
    bpg_rsp_manage_t mgr;
    nm_node_t mgr_node;

    assert(app);
    assert(logic_mgr);

    if (name == 0) name = cpe_hs_data((cpe_hash_string_t)&s_bpg_rsp_manage_default_name);
    if (em == 0) em = gd_app_em(app);

    if (logic_mgr == 0) {
        CPE_ERROR(em, "%s: create: logic_mgr not exist!", name);
        return NULL;
    }

    mgr_node = nm_group_create(gd_app_nm_mgr(app), name, sizeof(struct bpg_rsp_manage));
    if (mgr_node == NULL) return NULL;

    mgr = (bpg_rsp_manage_t)nm_node_data(mgr_node);

    mgr->m_app = app;
    mgr->m_alloc = gd_app_alloc(app);
    mgr->m_logic_mgr = logic_mgr;
    mgr->m_em = em;
    mgr->m_flags = 0;

    mgr->m_ctx_capacity = 0;
    mgr->m_ctx_init = NULL;
    mgr->m_ctx_fini = NULL;
    mgr->m_pkg_init = NULL;
    mgr->m_ctx_ctx = NULL;

    mgr->m_rsp_max_size = 4 * 1024;
    mgr->m_rsp_buf = NULL;

    mgr->m_debug = 0;

    mgr->m_commit_dsp = NULL;
    mgr->m_forward_dsp = NULL;

    TAILQ_INIT(&mgr->m_pkg_builders);

    nm_node_set_type(mgr_node, &s_nm_node_type_bpg_rsp_manage);

    return mgr;
}

static void bpg_rsp_manage_clear(nm_node_t node) {
    bpg_rsp_manage_t mgr;
    mgr = (bpg_rsp_manage_t)nm_node_data(node);

    while(!TAILQ_EMPTY(&mgr->m_pkg_builders)) {
        bpg_rsp_pkg_builder_free(TAILQ_FIRST(&mgr->m_pkg_builders));
    }

    if (mgr->m_rsp_buf) {
        bpg_pkg_free(mgr->m_rsp_buf);
        mgr->m_rsp_buf = NULL;
    }

    if (mgr->m_commit_dsp) {
        bpg_pkg_dsp_free(mgr->m_commit_dsp);
        mgr->m_commit_dsp = NULL;
    }

    if (mgr->m_forward_dsp) {
        bpg_pkg_dsp_free(mgr->m_forward_dsp);
        mgr->m_forward_dsp = NULL;
    }
}

void bpg_rsp_manage_free(bpg_rsp_manage_t mgr) {
    nm_node_t mgr_node;
    assert(mgr);

    mgr_node = nm_node_from_data(mgr);
    if (nm_node_type(mgr_node) != &s_nm_node_type_bpg_rsp_manage) return;
    nm_node_free(mgr_node);
}

bpg_rsp_manage_t
bpg_rsp_manage_find(gd_app_context_t app, cpe_hash_string_t name) {
    nm_node_t node;

    if (name == NULL) name = (cpe_hash_string_t)&s_bpg_rsp_manage_default_name;

    node = nm_mgr_find_node(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_bpg_rsp_manage) return NULL;
    return (bpg_rsp_manage_t)nm_node_data(node);
}

bpg_rsp_manage_t
bpg_rsp_manage_find_nc(gd_app_context_t app, const char * name) {
    nm_node_t node;

    if (name == NULL) return bpg_rsp_manage_default(app);

    node = nm_mgr_find_node_nc(gd_app_nm_mgr(app), name);
    if (node == NULL || nm_node_type(node) != &s_nm_node_type_bpg_rsp_manage) return NULL;
    return (bpg_rsp_manage_t)nm_node_data(node);
}

bpg_rsp_manage_t
bpg_rsp_manage_default(gd_app_context_t app) {
    return bpg_rsp_manage_find(app, (cpe_hash_string_t)&s_bpg_rsp_manage_default_name);
}

gd_app_context_t bpg_rsp_manage_app(bpg_rsp_manage_t mgr) {
    return mgr->m_app;
}

const char * bpg_rsp_manage_name(bpg_rsp_manage_t mgr) {
    return nm_node_name(nm_node_from_data(mgr));
}

cpe_hash_string_t
bpg_rsp_manage_name_hs(bpg_rsp_manage_t mgr) {
    return nm_node_name_hs(nm_node_from_data(mgr));
}

bpg_pkg_dsp_t bpg_rsp_manage_commit_dsp(bpg_rsp_manage_t mgr) {
    return mgr->m_commit_dsp;
}

void bpg_rsp_manage_set_commit_dsp(bpg_rsp_manage_t mgr, bpg_pkg_dsp_t dsp) {
    if (mgr->m_commit_dsp) bpg_pkg_dsp_free(mgr->m_commit_dsp);
    mgr->m_commit_dsp = dsp;
}

bpg_pkg_dsp_t bpg_rsp_manage_forward_dsp(bpg_rsp_manage_t mgr) {
    return mgr->m_forward_dsp;
}

void bpg_rsp_manage_set_forward_dsp(bpg_rsp_manage_t mgr, bpg_pkg_dsp_t dsp) {
    if (mgr->m_forward_dsp) bpg_pkg_dsp_free(mgr->m_forward_dsp);
    mgr->m_forward_dsp = dsp;
}

bpg_pkg_t
bpg_rsp_manage_rsp_buf(bpg_rsp_manage_t mgr, const char * pkg_mgr_name, LPDRMETA carry_meta, size_t carry_capacity) {
    if (mgr->m_rsp_buf) {
        if (strcmp(bpg_pkg_manage_name(bpg_pkg_mgr(mgr->m_rsp_buf)), pkg_mgr_name) != 0
            || bpg_pkg_carry_data_meta(mgr->m_rsp_buf) != carry_meta
            || bpg_pkg_carry_data_capacity(mgr->m_rsp_buf) < carry_capacity
            || bpg_pkg_pkg_capacity(mgr->m_rsp_buf) < mgr->m_rsp_max_size)
        {
            bpg_pkg_free(mgr->m_rsp_buf);
            mgr->m_rsp_buf = NULL;
        }
    }

    if (mgr->m_rsp_buf == NULL) {
        bpg_pkg_manage_t pkg_manage = bpg_pkg_manage_find_nc(mgr->m_app, pkg_mgr_name);
        if (pkg_manage == NULL) {
            CPE_ERROR(mgr->m_em, "bpg_rsp_manage_rsp_buf: bpg_pkg_manage %s not exist!", pkg_mgr_name);
            return NULL;
        }

        mgr->m_rsp_buf = bpg_pkg_create(pkg_manage, mgr->m_rsp_max_size, carry_meta, carry_capacity);
    }

    return mgr->m_rsp_buf;
}

uint32_t bpg_rsp_manage_flags(bpg_rsp_manage_t mgr) {
    return mgr->m_flags;
}

void bpg_rsp_manage_flags_set(bpg_rsp_manage_t mgr, uint32_t flag) {
    mgr->m_flags = flag;
}

void bpg_rsp_manage_flag_enable(bpg_rsp_manage_t mgr, bpg_rsp_manage_flag_t flag) {
    mgr->m_flags |= flag;
}

void bpg_rsp_manage_flag_disable(bpg_rsp_manage_t mgr, bpg_rsp_manage_flag_t flag) {
    mgr->m_flags &= ~((uint32_t)flag);
}

int bpg_rsp_manage_flag_is_enable(bpg_rsp_manage_t mgr, bpg_rsp_manage_flag_t flag) {
    return mgr->m_flags & flag;
}

void bpg_rsp_manage_set_context_op(
    bpg_rsp_manage_t mgr,
    size_t ctx_capacity,
    bpg_logic_ctx_init_fun_t ctx_init,
    bpg_logic_ctx_fini_fun_t ctx_fini,
    bpg_logic_pkg_init_fun_t pkg_init,
    void * ctx_ctx)
{
    mgr->m_ctx_capacity = ctx_capacity;
    mgr->m_ctx_init = ctx_init;
    mgr->m_ctx_fini = ctx_fini;
    mgr->m_pkg_init = pkg_init;
    mgr->m_ctx_ctx = ctx_ctx;
}
