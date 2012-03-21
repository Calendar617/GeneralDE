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

static int bpg_rsp_copy_ctx_to_pkg(bpg_rsp_t rsp, logic_context_t op_context, size_t capacity, error_monitor_t em);

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

    if (bpg_rsp_copy_ctx_to_pkg(bpg_rsp, op_context, bpg_pkg_pkg_capacity(response_buf), em) != 0) return;

    if (gd_dp_dispatch_by_string(bpg_mgr->m_commit_to, bpg_pkg_to_dp_req(response_buf), em) != 0) {
        CPE_ERROR(em, "%s.%s: bpg_rsp_commit: dispatch fail!", bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }
}

int bpg_rsp_copy_ctx_to_pkg(
    bpg_rsp_t rsp, logic_context_t op_context,
    size_t data_capacity,
    error_monitor_t em)
{
    return 0;
}
