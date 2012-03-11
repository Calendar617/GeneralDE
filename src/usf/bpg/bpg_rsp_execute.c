#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/dp/dp_request.h"
#include "usf/logic/logic_context.h"
#include "protocol/base/base_package.h"
#include "bpg_internal_ops.h"

extern LPDRMETALIB g_metalib_base_package;

int bpg_rsp_execute(gd_dp_req_t req, void * ctx, error_monitor_t em) {
    struct basepkg * pkg;
    bpg_rsp_t bpg_rsp;
    bpg_manage_t bpg_mgr;
    logic_context_t op_context;

    bpg_rsp = (bpg_rsp_t)ctx;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    pkg = (struct basepkg *)gd_dp_req_data(req);
    if (pkg == NULL) {
        CPE_ERROR(em, "bpg_rsp_execute: input dp_req data is NULL!");
        return 0;
    }

    if (gd_dp_req_size(req) < sizeof(struct basepkg_head)) {
        CPE_ERROR(
            em, "bpg_rsp_execute: input dp_req size is too small, head size is %d, but input size is %d!",
            (int)sizeof(struct basepkg_head), (int)gd_dp_req_size(req));
        return 0;
    }

    op_context =
        logic_context_create(
            bpg_mgr->m_logic_mgr,
            bpg_manage_flag_is_enable(bpg_mgr, bpg_manage_flag_sn_use_client)
            ? pkg->head.sn
            : INVALID_LOGIC_CONTEXT_ID,
            bpg_mgr->m_ctx_capacity);
    if (op_context == NULL) {
        CPE_ERROR(em, "bpg_rsp_execute: create context fail, capacity is %d!", (int)bpg_mgr->m_ctx_capacity);
        return 0;
    }

    return 0;
}
