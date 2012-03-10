#include <assert.h>
#include "cpe/cfg/cfg_read.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "usf/logic/logic_executor.h"
#include "usf/logic/logic_executor_build.h"
#include "usf/bpg/bpg_rsp.h"
#include "usf/bpg/bpg_manage.h"
#include "bpg_internal_ops.h"

static void bpg_rsp_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_bpg_rsp = {
    "usf_bpg_rsp",
    bpg_rsp_clear
};

bpg_rsp_t bpg_rsp_create(bpg_manage_t mgr, cfg_t cfg) {
    bpg_rsp_t rsp;
    gd_nm_node_t rsp_node;
    const char * name;
    cfg_t cfg_executor;

    assert(mgr);
    if (cfg == 0) return NULL;

    name = cfg_get_string(cfg, "name", NULL);
    if (name == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp: no name configured!", bpg_manage_name(mgr)) ;
        return NULL;
    }

    cfg_executor = cfg_find_cfg(cfg, "operations");
    if (cfg_executor == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp %s: no executor configured!", bpg_manage_name(mgr), name) ;
        return NULL;
    }

    rsp_node = gd_nm_instance_create(gd_app_nm_mgr(mgr->m_app), name, sizeof(struct bpg_manage));
    if (rsp_node == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp %s: create fail, maybe name duplicate!", bpg_manage_name(mgr), name) ;
        return NULL;
    }

    rsp = (bpg_rsp_t)gd_nm_node_data(rsp_node);
    rsp->m_mgr = mgr;
    rsp->m_flags = 0;

    rsp->m_executor = NULL;

    if (cfg_get_int32(cfg, "debug", 0)) bpg_rsp_flag_enable(rsp, bpg_rsp_flag_debug);

    gd_nm_node_set_type(rsp_node, &s_nm_node_type_bpg_rsp);
    return rsp;
}

const char * bpg_rsp_name(bpg_rsp_t rsp) {
    return gd_nm_node_name(gd_nm_node_from_data(rsp));
}

cpe_hash_string_t
bpg_rsp_name_hs(bpg_rsp_t rsp) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(rsp));
}

uint32_t bpg_rsp_flags(bpg_rsp_t rsp) {
    return rsp->m_flags;
}

void bpg_rsp_flags_set(bpg_rsp_t rsp, uint32_t flag) {
    rsp->m_flags = flag;
}

void bpg_rsp_flag_enable(bpg_rsp_t rsp, bpg_rsp_flag_t flag) {
    rsp->m_flags |= flag;
}

void bpg_rsp_flag_disable(bpg_rsp_t rsp, bpg_rsp_flag_t flag) {
    rsp->m_flags &= ~((uint32_t)flag);
}

int bpg_rsp_flag_is_enable(bpg_rsp_t rsp, bpg_rsp_flag_t flag) {
    return rsp->m_flags & flag;
}

static void bpg_rsp_clear(gd_nm_node_t node) {
    bpg_rsp_t rsp;
    rsp = (bpg_rsp_t)gd_nm_node_data(node);

    if (rsp->m_executor) {
        logic_executor_free(rsp->m_executor);
        rsp->m_executor = NULL;
    }
}
