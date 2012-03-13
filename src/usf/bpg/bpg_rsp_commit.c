#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/dp/dp_request.h"
#include "gd/dp/dp_manage.h"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "usf/bpg/bpg_manage.h"
#include "usf/bpg/bpg_req.h"
#include "usf/bpg/bpg_rsp.h"
#include "protocol/base/base_package.h"
#include "protocol/base/base_package_internal.h"
#include "bpg_internal_ops.h"

extern LPDRMETALIB g_metalib_base_package;

static int bpg_rsp_copy_ctx_to_pkg(bpg_rsp_t rsp, logic_context_t op_context, struct basepkg * pkg, size_t capacity, error_monitor_t em);

void bpg_rsp_commit(logic_context_t op_context, void * user_data) {
    struct basepkg * pkg;
    bpg_rsp_t bpg_rsp;
    bpg_manage_t bpg_mgr;
    bpg_req_t response_buf;
    error_monitor_t em;
    logic_data_t bpg_private_data;
    struct bpg_carry_info * bpg_private;
    logic_data_t bpg_carry_data;

    bpg_rsp = (bpg_rsp_t)user_data;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    em = bpg_mgr->m_em;

    if (bpg_mgr->m_send_to == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_commit: no send-to configured, ignore commit!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }

    bpg_private_data = logic_data_find(op_context, "bpg_carry_info");
    if (bpg_private_data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_commit: no bpg_carry_info in context!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }

    bpg_private = (struct bpg_carry_info *)logic_data_data(bpg_private_data);
    bpg_carry_data = NULL;
    if (bpg_private->carry_meta_name[0] != 0) {
        bpg_carry_data = logic_data_find(op_context, bpg_private->carry_meta_name);
        if (bpg_carry_data == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_commit: no caary data %s in context!",
                bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), bpg_private->carry_meta_name);
            return;
        }
    }

    response_buf =
        bpg_manage_rsp_buf(
            bpg_mgr,
            bpg_carry_data ? logic_data_meta(bpg_carry_data) : NULL,
            bpg_carry_data ? logic_data_capacity(bpg_carry_data): 0);
    if (response_buf == NULL) {
        CPE_ERROR(em, "%s: bpg_rsp_commit: response buf is NULL!", bpg_manage_name(bpg_mgr));
        return;
    }

    bpg_req_set_convertor(
        response_buf,
        (bpg_data_convert_fun_t)(ptr_int_t)bpg_private->cvt_encode,
        (bpg_data_convert_fun_t)(ptr_int_t)bpg_private->cvt_decode,
        (void *)(ptr_int_t)bpg_private->cvt_ctx);

    if (bpg_req_pkg_capacity(response_buf) < sizeof(struct basepkg_head)) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_commit: response buf size is too small, head size is %d, but response buf size is %d!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp),
            (int)sizeof(struct basepkg_head), (int)bpg_req_pkg_capacity(response_buf))
        return;
    }

    pkg = (struct basepkg *)bpg_req_pkg_data(response_buf);
    bzero(pkg, sizeof(struct basepkg));

    pkg->head.magic = BASEPKG_HEAD_MAGIC;
    pkg->head.version = 1;
    pkg->head.sn = logic_context_id(op_context);
    pkg->head.clientId = bpg_private->clientId;
    pkg->head.errno = logic_context_errno(op_context);
    pkg->head.cmd = bpg_private->cmd;

    if (bpg_rsp_copy_ctx_to_pkg(bpg_rsp, op_context, pkg, bpg_req_pkg_capacity(response_buf), em) != 0) return;

    if (gd_dp_dispatch_by_string(bpg_mgr->m_send_to, bpg_req_to_dp_req(response_buf), em) != 0) {
        CPE_ERROR(em, "%s.%s: bpg_rsp_commit: dispatch fail!", bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return;
    }
}

int bpg_rsp_copy_ctx_to_pkg(
    bpg_rsp_t rsp, logic_context_t op_context,
    struct basepkg * pkg, size_t data_capacity,
    error_monitor_t em)
{
    return 0;
}
