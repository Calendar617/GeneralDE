#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/dp/dp_request.h"
#include "gd/dp/dp_manage.h"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "usf/bpg_rsp/bpg_rsp.h"
#include "protocol/bpg_rsp_carry_info.h"
#include "bpg_rsp_internal_ops.h"

static int bpg_rsp_commit_build_pkg(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg, error_monitor_t em);

void bpg_rsp_commit(logic_context_t op_context, void * user_data) {
    bpg_rsp_t bpg_rsp;
    bpg_rsp_manage_t bpg_mgr;
    bpg_pkg_t response_buf;
    error_monitor_t em;
    logic_data_t bpg_private_data;
    struct bpg_carry_info * bpg_private;
    logic_data_t bpg_carry_data;

    bpg_rsp = (bpg_rsp_t)user_data;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    em = bpg_mgr->m_em;

    if (bpg_mgr->m_commit_to == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_commit: no send-to configured, ignore commit!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }

    bpg_private_data = logic_data_find(op_context, "bpg_carry_info");
    if (bpg_private_data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_commit: no bpg_carry_info in context!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }

    bpg_private = (struct bpg_carry_info *)logic_data_data(bpg_private_data);
    bpg_carry_data = NULL;
    if (bpg_private->carry_meta_name[0] != 0) {
        bpg_carry_data = logic_data_find(op_context, bpg_private->carry_meta_name);
        if (bpg_carry_data == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_commit: no caary data %s in context!",
                bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), bpg_private->carry_meta_name);
            return;
        }
    }

    response_buf =
        bpg_rsp_manage_rsp_buf(
            bpg_mgr,
            bpg_private->pkg_mgr_name,
            bpg_carry_data ? logic_data_meta(bpg_carry_data) : NULL,
            bpg_carry_data ? logic_data_capacity(bpg_carry_data): 0);
    if (response_buf == NULL) {
        CPE_ERROR(em, "%s: bpg_rsp_commit: response buf is NULL!", bpg_rsp_manage_name(bpg_mgr));
        return;
    }

    bpg_pkg_init(response_buf);
    bpg_pkg_set_sn(response_buf, logic_context_id(op_context));
    bpg_pkg_set_client_id(response_buf, bpg_private->clientId);
    bpg_pkg_set_errno(response_buf, logic_context_errno(op_context));
    bpg_pkg_set_cmd(response_buf, bpg_private->cmd);
    bpg_pkg_set_connection_id(response_buf, bpg_private->connectionId);

    if (bpg_rsp_commit_build_pkg(bpg_rsp, op_context, response_buf, em) != 0) return;

    if (gd_dp_dispatch_by_string(bpg_mgr->m_commit_to, bpg_pkg_to_dp_req(response_buf), em) != 0) {
        CPE_ERROR(em, "%s.%s: bpg_rsp_commit: dispatch fail!", bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }
}

static int bpg_rsp_commit_build_pkg_append_info_from_ctx(
    bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg, const char * copy_data_name, LPDRMETALIB metalib, error_monitor_t em)
{
    logic_data_t data;
    LPDRMETA data_meta;

    data = logic_data_find(op_context, copy_data_name);
    if (data == NULL) return -1;

    data_meta = dr_lib_find_meta_by_name(metalib, copy_data_name);
    if (data_meta == NULL) {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: %s: meta not exist!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), copy_data_name);
        return 0;
    }

    if (bpg_pkg_add_append_data(
            pkg, data_meta, 
            logic_data_data(data), logic_data_capacity(data),
            em) != 0)
    {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: %s: append data to pkg fauil!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), copy_data_name);
        return 0;
    }

    if (rsp->m_mgr->m_debug) {
        CPE_INFO(
            em, "%s.%s: copy_ctx_to_pdu: %s: append data to pkg from context success!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), copy_data_name);
    }

    return 0;
}

static int bpg_rsp_commit_build_pkg_append_info_from_builder(
    bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg,
    const char * data_name, error_monitor_t em)
{
    bpg_rsp_pkg_builder_t pkg_builder;

    TAILQ_FOREACH(pkg_builder, &rsp->m_mgr->m_pkg_builders, m_next) {
        if (pkg_builder->m_build_fun) {
            switch(pkg_builder->m_build_fun(pkg, op_context, pkg_builder->m_build_ctx)) {
            case bpg_pkg_build_result_success: {
                if (rsp->m_mgr->m_debug) {
                    CPE_INFO(
                        em, "%s.%s: copy_ctx_to_pdu: %s: append data to pkg from context success!",
                        bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), data_name);
                }
                return 0;
            }
            case bpg_pkg_build_result_fail:
                return 0;
            case bpg_pkg_build_result_unknown:
                continue;
            }
        }
    }

    return -1;
}

static int bpg_rsp_commit_build_pkg_append_info(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg, error_monitor_t em) {
    struct bpg_rsp_copy_info * copy_info;

    LPDRMETALIB metalib = bpg_pkg_data_meta_lib(pkg);
    if (metalib == NULL) {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: no metalib!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp));
        return -1;
    }

    TAILQ_FOREACH(copy_info, &rsp->m_ctx_to_pdu, m_next) {
        const char * copy_data_name;
        copy_data_name = bpg_rsp_copy_info_data(copy_info);

        if (bpg_rsp_commit_build_pkg_append_info_from_ctx(rsp, op_context, pkg, copy_data_name, metalib, em) == 0) {
            continue;
        }

        if (bpg_rsp_commit_build_pkg_append_info_from_builder(rsp, op_context, pkg, copy_data_name, em) == 0) {
            continue;
        }

        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: %s: can`t find data!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), copy_data_name);
    }

    return 0;
}

static int bpg_rsp_commit_build_pkg_main_info(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg, error_monitor_t em) {
    logic_data_t data;
    LPDRMETA meta;

    meta = bpg_pkg_main_data_meta(pkg, em);
    if (meta == NULL) {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: main: can`t find meta!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp));
        return -1;
    }

    data = logic_data_find(op_context, dr_meta_name(meta));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: main: can`t find %s from ctx!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), dr_meta_name(meta));
        return -1;
    }

    if (bpg_pkg_set_main_data(pkg, meta, logic_data_data(data), logic_data_capacity(data), em) != 0) {
        CPE_ERROR(
            em, "%s.%s: copy_ctx_to_pdu: main: set data fail, meta = %s!",
            bpg_rsp_manage_name(rsp->m_mgr), bpg_rsp_name(rsp), dr_meta_name(meta));
        return -1;
    }
            
    return 0;
}

int bpg_rsp_commit_build_pkg(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t pkg, error_monitor_t em) {
    if (bpg_rsp_commit_build_pkg_main_info(rsp, op_context, pkg, em) != 0) return -1;
    if (bpg_rsp_commit_build_pkg_append_info(rsp, op_context, pkg, em) != 0) return -1;

    return 0;
}
