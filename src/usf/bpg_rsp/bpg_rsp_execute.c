#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
#include "gd/dp/dp_request.h"
#include "gd/dp/dp_manage.h"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "usf/bpg_pkg/bpg_pkg.h"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "usf/bpg_rsp/bpg_rsp_manage.h"
#include "usf/bpg_rsp/bpg_rsp.h"
#include "protocol/bpg_rsp_carry_info.h"
#include "bpg_rsp_internal_ops.h"

static int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t req, error_monitor_t em);
static int bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t bpg_req, error_monitor_t em);
static int bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t bpg_req, error_monitor_t em);

int bpg_rsp_execute(gd_dp_req_t dp_req, void * ctx, error_monitor_t em) {
    bpg_rsp_t bpg_rsp;
    bpg_rsp_manage_t bpg_mgr;
    logic_context_t op_context;
    bpg_pkg_t req;

    bpg_rsp = (bpg_rsp_t)ctx;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    if (bpg_pkg_pkg_data(req) == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: input dp_req data is NULL!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return 0;
    }

    req = bpg_pkg_from_dp_req(dp_req);
    if (req == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: input req is not a bpg_req, type is %s!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), gd_dp_req_type(dp_req));
        return 0;
    }

    op_context =
        logic_context_create(
            bpg_mgr->m_logic_mgr,
            bpg_rsp_manage_flag_is_enable(bpg_mgr, bpg_rsp_manage_flag_sn_use_client)
            ? bpg_pkg_sn(req)
            : INVALID_LOGIC_CONTEXT_ID,
            bpg_mgr->m_ctx_capacity);
    if (op_context == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: create context fail, capacity is %d!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), (int)bpg_mgr->m_ctx_capacity);
        return 0;
    }

    if (bpg_rsp_copy_pkg_to_ctx(bpg_rsp, op_context, req, em) != 0) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy data from pdu to context!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        logic_context_free(op_context);
        return 0;
    }

    if (logic_context_bind(op_context, bpg_rsp->m_executor) != 0) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: bind executor to context fail!",
            bpg_rsp_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        logic_context_free(op_context);
        return 0;
    }

    if (bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp, op_context, req, em) != 0) {
        logic_context_free(op_context);
        return 0;
    }

    if (bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp, op_context, req, em) != 0) {
        logic_context_free(op_context);
        return 0;
    }

    logic_context_set_commit(op_context, bpg_rsp_commit, bpg_rsp);

    logic_context_execute(op_context);

    return 0;
}

static int bpg_rsp_copy_main_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t req, error_monitor_t em) {
    LPDRMETA data_meta;
    logic_data_t data;
    bpg_rsp_manage_t mgr;
    size_t output_size;

    mgr = rsp->m_mgr;

    data_meta = bpg_pkg_main_data_meta(req, em);
    if (data_meta == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: get data meta fail!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp));
        return -1;
    }

    data = logic_data_get_or_create(op_context, data_meta, bpg_pkg_body_origin_len(req));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: %s create data fail, capacity=%d!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), dr_meta_name(data_meta), bpg_pkg_body_origin_len(req));
        return -1;
    }

    output_size = logic_data_capacity(data);
    if (bpg_pkg_get_main_data(
            req,
            data_meta,
            logic_data_data(data), &output_size,
            em) != 0)
    {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: %s decode data fail!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), 
            dr_meta_name(data_meta));
        return -1;
    }

    return 0;
}

static int bpg_rsp_copy_append_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t req, error_monitor_t em) {
    LPDRMETA data_meta;
    logic_data_t data;
    int i;
    bpg_rsp_manage_t mgr;
    int32_t append_info_count;
    size_t output_size;

    mgr = rsp->m_mgr;
    assert(mgr);

    append_info_count = bpg_pkg_append_info_count(req);
    for(i = 0; i < append_info_count; ++i) {
        bpg_pkg_append_info_t append_info;

        append_info = bpg_pkg_append_info_at(req, i);
        if (append_info == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: append info is NULL!",
                bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), i);
            continue;
        }

        data_meta = bpg_pkg_append_data_meta(req, append_info, em);
        if (data_meta == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: append meta not exist!",
                bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), i);
            return -1;
        }

        data = logic_data_get_or_create(op_context, data_meta, bpg_pkg_append_info_origin_size(append_info));
        if (data == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s create data fail, capacity=%d!",
                bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), i, dr_meta_name(data_meta), bpg_pkg_append_info_origin_size(append_info));
            return -1;
        }

        output_size = logic_data_capacity(data);
        if (bpg_pkg_get_append_data(
                req, append_info, data_meta,
                logic_data_data(data), &output_size,
                em) != 0)
        {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s decode data fail!",
                bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp), i, dr_meta_name(data_meta));
            return -1;
        }
    }

    return 0;
}

int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t req, error_monitor_t em) {
    bpg_rsp_manage_t mgr;

    mgr = rsp->m_mgr;

    if (bpg_rsp_copy_main_to_ctx(rsp, op_context, req, em) != 0) return -1;
    if (bpg_rsp_copy_append_to_ctx(rsp, op_context, req, em) != 0) return -1;

    return 0;
}

int bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t bpg_req, error_monitor_t em) {
    LPDRMETA carry_meta;
    logic_data_t data;
    bpg_rsp_manage_t mgr;
    mgr = rsp->m_mgr;

    carry_meta = bpg_pkg_carry_data_meta(bpg_req);
    if (carry_meta == NULL) return 0;

    data = logic_data_get_or_create(op_context, carry_meta, bpg_pkg_carry_data_size(bpg_req));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_carry_data: %s create data fail, size=%d!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp),
            dr_meta_name(carry_meta), (int)bpg_pkg_carry_data_size(bpg_req));
        return -1;
    }

    memcpy(logic_data_data(data), bpg_pkg_carry_data(bpg_req), bpg_pkg_carry_data_size(bpg_req));
    return 0;
}

extern char g_metalib_caary_package[];

int bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_pkg_t bpg_req, error_monitor_t em) {
    LPDRMETA bpg_carry_data_meta;
    logic_data_t data;
    struct bpg_carry_info * buf;
    bpg_rsp_manage_t mgr;

    mgr = rsp->m_mgr;

    bpg_carry_data_meta = dr_lib_find_meta_by_name((LPDRMETALIB)g_metalib_caary_package, "bpg_carry_info");
    if (bpg_carry_data_meta == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_bpg_carry_data: bpg_carry_info meta not exist!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp));
        return -1;
    }

    data = logic_data_get_or_create(op_context, bpg_carry_data_meta, dr_meta_size(bpg_carry_data_meta));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_bpg_carry_data: %s create data fail, size=%d!",
            bpg_rsp_manage_name(mgr), bpg_rsp_name(rsp),
            dr_meta_name(bpg_carry_data_meta), (int)dr_meta_size(bpg_carry_data_meta));
        return -1;
    }

    buf = (struct bpg_carry_info *)logic_data_data(data);
    buf->clientId = bpg_pkg_client_id(bpg_req);
    buf->connectionId = bpg_pkg_connection_id(bpg_req);
    buf->cmd = bpg_pkg_cmd(bpg_req);
    buf->carry_data_size = bpg_pkg_carry_data_size(bpg_req);

    if (bpg_pkg_carry_data_meta(bpg_req)) {
        strncpy(buf->carry_meta_name, dr_meta_name(bpg_pkg_carry_data_meta(bpg_req)), sizeof(buf->carry_meta_name));
    }
    else {
        buf->carry_meta_name[0] = 0;
    }

    strncpy(buf->pkg_mgr_name, bpg_pkg_manage_name(bpg_pkg_mgr(bpg_req)), sizeof(buf->pkg_mgr_name));

    return 0;
}
