#include <assert.h>
#include "cpe/cfg/cfg_read.h"
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "cpe/dp/dp_manage.h"
#include "cpe/dp/dp_responser.h"
#include "usf/logic/logic_executor.h"
#include "usf/logic/logic_executor_build.h"
#include "usf/logic/logic_executor_type.h"
#include "usf/bpg_rsp/bpg_rsp.h"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "bpg_rsp_internal_ops.h"

static void bpg_rsp_clear(nm_node_t node);

struct nm_node_type s_nm_node_type_bpg_rsp = {
    "usf_bpg_rsp",
    bpg_rsp_clear
};

static int bpg_rsp_read_respons_copy_infos(bpg_rsp_t bpg_rsp, cfg_t cfg) {
    struct cfg_it it;
    cfg_t child_cfg;
    cfg_it_init(&it, cfg);
    
    while((child_cfg = cfg_it_next(&it))) {
        const char * write_data_name = cfg_as_string(child_cfg, NULL);
        if (write_data_name == NULL) {
            CPE_ERROR(
                bpg_rsp->m_mgr->m_em,
                "%s: create rsp %s: read response-data fail!",
                bpg_rsp_manage_name(bpg_rsp->m_mgr), bpg_rsp_name(bpg_rsp));
            return -1;
        }

        if (bpg_rsp_copy_info_create(bpg_rsp->m_mgr, &bpg_rsp->m_ctx_to_pdu, write_data_name) == NULL) {
            CPE_ERROR(
                bpg_rsp->m_mgr->m_em,
                "%s: create rsp %s: crate response-data %s!",
                bpg_rsp_manage_name(bpg_rsp->m_mgr), bpg_rsp_name(bpg_rsp), write_data_name);
            return -1;
        }
    }

    return 0;
}

static int bpg_rsp_create_dp_rsp_and_bind(bpg_rsp_t bpg_rsp, cfg_t cfg) {
    dp_rsp_t dp_rsp;
    cfg_t cfg_respons;

    cfg_respons = cfg_find_cfg(cfg, "respons-to");
    if (cfg_respons == NULL) {
        CPE_ERROR(
            bpg_rsp->m_mgr->m_em,
            "%s: create rsp %s: create dp_rsp fail, maybe name duplicate!",
            bpg_rsp_manage_name(bpg_rsp->m_mgr), bpg_rsp_name(bpg_rsp));
        return -1;
    }

    dp_rsp = dp_rsp_create(
        gd_app_dp_mgr(bpg_rsp->m_mgr->m_app),
        bpg_rsp_name(bpg_rsp));
    if (dp_rsp == NULL) {
        CPE_ERROR(
            bpg_rsp->m_mgr->m_em,
            "%s: create rsp %s: create dp_rsp fail, maybe name duplicate!",
            bpg_rsp_manage_name(bpg_rsp->m_mgr), bpg_rsp_name(bpg_rsp));
        return -1;
    }

    dp_rsp_set_processor(dp_rsp, bpg_rsp_execute, bpg_rsp);

    if (dp_rsp_bind_by_cfg(dp_rsp, cfg_respons, bpg_rsp->m_mgr->m_em) != 0) {
        CPE_ERROR(
            bpg_rsp->m_mgr->m_em,
            "%s: create rsp %s: bind rsps by cfg fail!",
            bpg_rsp_manage_name(bpg_rsp->m_mgr), bpg_rsp_name(bpg_rsp));
        dp_rsp_free(dp_rsp);
        return -1;
    }
    else {
        return 0;
    }
}

bpg_rsp_t bpg_rsp_create(bpg_rsp_manage_t mgr, cfg_t cfg) {
    bpg_rsp_t rsp;
    nm_node_t rsp_node;
    const char * name;
    cfg_t cfg_executor;
    const char * group_name;
    logic_executor_type_group_t type_group;

    assert(mgr);
    if (cfg == 0) return NULL;

    name = cfg_get_string(cfg, "name", NULL);
    if (name == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp: no name configured!", bpg_rsp_manage_name(mgr)) ;
        return NULL;
    }

    group_name = cfg_get_string(cfg, "operations-from", NULL);
    type_group = logic_executor_type_group_find_nc(mgr->m_app, group_name);
    if (type_group == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: create rsp %s: executor_type_group '%s' not exist! (read from operations-from)",
            bpg_rsp_manage_name(mgr), name, (group_name ? group_name : "default")) ;
        return NULL;
    }

    cfg_executor = cfg_find_cfg(cfg, "operations");
    if (cfg_executor == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp %s: no executor configured! (read from operations)", bpg_rsp_manage_name(mgr), name) ;
        return NULL;
    }

    rsp_node = nm_instance_create(gd_app_nm_mgr(mgr->m_app), name, sizeof(struct bpg_rsp_manage));
    if (rsp_node == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp %s: create fail, maybe name duplicate!", bpg_rsp_manage_name(mgr), name) ;
        return NULL;
    }

    rsp = (bpg_rsp_t)nm_node_data(rsp_node);
    rsp->m_mgr = mgr;
    rsp->m_flags = 0;
    TAILQ_INIT(&rsp->m_ctx_to_pdu);

    if (bpg_rsp_read_respons_copy_infos(rsp, cfg_find_cfg(cfg, "response-data")) != 0) {
        nm_node_free(rsp_node);
        return NULL;
    }

    rsp->m_executor = logic_executor_build(
        mgr->m_logic_mgr,
        cfg_executor,
        type_group,
        mgr->m_em);
    if (rsp->m_executor == NULL) {
        CPE_ERROR(mgr->m_em, "%s: create rsp %s: create executor fail!", bpg_rsp_manage_name(mgr), name) ;
        bpg_rsp_copy_info_clear(rsp->m_mgr, &rsp->m_ctx_to_pdu);
        nm_node_free(rsp_node);
        return NULL;
    }

    if (cfg_get_int32(cfg, "debug", 0)) bpg_rsp_flag_enable(rsp, bpg_rsp_flag_debug);

    if (bpg_rsp_create_dp_rsp_and_bind(rsp, cfg) != 0) {
        logic_executor_free(rsp->m_executor);
        bpg_rsp_copy_info_clear(rsp->m_mgr, &rsp->m_ctx_to_pdu);
        nm_node_free(rsp_node);
        return NULL;
    }

    nm_node_set_type(rsp_node, &s_nm_node_type_bpg_rsp);
    return rsp;
}

void bpg_rsp_free(bpg_rsp_t rsp) {
    nm_node_t rsp_node;
    assert(rsp);

    rsp_node = nm_node_from_data(rsp);
    if (nm_node_type(rsp_node) != &s_nm_node_type_bpg_rsp) return;
    nm_node_free(rsp_node);
}

const char * bpg_rsp_name(bpg_rsp_t rsp) {
    return nm_node_name(nm_node_from_data(rsp));
}

cpe_hash_string_t
bpg_rsp_name_hs(bpg_rsp_t rsp) {
    return nm_node_name_hs(nm_node_from_data(rsp));
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

static void bpg_rsp_clear(nm_node_t node) {
    bpg_rsp_t bpg_rsp;
    dp_rsp_t dp_rsp;

    bpg_rsp = (bpg_rsp_t)nm_node_data(node);

    bpg_rsp_copy_info_clear(bpg_rsp->m_mgr, &bpg_rsp->m_ctx_to_pdu);

    if (bpg_rsp->m_executor) {
        logic_executor_free(bpg_rsp->m_executor);
        bpg_rsp->m_executor = NULL;
    }

    dp_rsp = dp_rsp_find_by_name(gd_app_dp_mgr(bpg_rsp->m_mgr->m_app), bpg_rsp_name(bpg_rsp));
    if (dp_rsp) {
        dp_rsp_free(dp_rsp);
    }
}
