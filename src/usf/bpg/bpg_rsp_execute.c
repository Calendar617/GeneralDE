#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cvt.h"
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

static int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t req, struct basepkg * pkg, error_monitor_t em);
static int bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t bpg_req, error_monitor_t em);
static int bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t bpg_req, struct basepkg * pkg, error_monitor_t em);

int bpg_rsp_execute(gd_dp_req_t dp_req, void * ctx, error_monitor_t em) {
    struct basepkg * pkg;
    bpg_rsp_t bpg_rsp;
    bpg_manage_t bpg_mgr;
    logic_context_t op_context;
    bpg_req_t req;

    bpg_rsp = (bpg_rsp_t)ctx;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    pkg = (struct basepkg *)bpg_req_pkg_data(req);
    if (pkg == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: input dp_req data is NULL!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        return 0;
    }

    req = bpg_req_from_dp_req(dp_req);
    if (req == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: input req is not a bpg_req, type is %s!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), gd_dp_req_type(dp_req));
        return 0;
    }

    if (bpg_req_pkg_data_size(req) < sizeof(struct basepkg_head)) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: input dp_req size is too small, head size is %d, but input size is %d!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp),
            (int)sizeof(struct basepkg_head), (int)bpg_req_pkg_data_size(req));
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
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: create context fail, capacity is %d!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp), (int)bpg_mgr->m_ctx_capacity);
        return 0;
    }

    if (bpg_rsp_copy_pkg_to_ctx(bpg_rsp, op_context, req, pkg, em) != 0) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy data from pdu to context!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        logic_context_free(op_context);
        return 0;
    }

    if (logic_context_bind(op_context, bpg_rsp->m_executor) != 0) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: bind executor to context fail!",
            bpg_manage_name(bpg_mgr), bpg_rsp_name(bpg_rsp));
        logic_context_free(op_context);
        return 0;
    }

    if (bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp, op_context, req, em) != 0) {
        logic_context_free(op_context);
        return 0;
    }

    if (bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp, op_context, req, pkg, em) != 0) {
        logic_context_free(op_context);
        return 0;
    }

    logic_context_set_commit(op_context, bpg_rsp_commit, bpg_rsp);

    logic_context_execute(op_context);

    return 0;
}

static int bpg_rsp_copy_main_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t req, struct basepkg * pkg, error_monitor_t em) {
    int cmd_entry_idx;
    LPDRMETA data_meta;
    logic_data_t data;
    bpg_manage_t mgr;

    mgr = rsp->m_mgr;

    cmd_entry_idx = dr_meta_find_entry_idx_by_id(mgr->m_request_meta, pkg->head.cmd);
    if (cmd_entry_idx < 0) {
        if (logic_context_flag_is_enable(op_context, logic_context_flag_debug)) {
            CPE_INFO(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: request meta %s have no associate info of cmd %d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), mgr->m_request_meta_name, pkg->head.cmd);
        }
    }
    else {
        size_t input_size;
        size_t output_size;

        data_meta = dr_entry_self_meta(dr_meta_entry_at(mgr->m_request_meta, cmd_entry_idx));
        if (data_meta == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: %s.%s have no associate meta!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), 
                mgr->m_request_meta_name, dr_entry_name(dr_meta_entry_at(mgr->m_request_meta, cmd_entry_idx)));
            return -1;
        }

        data = logic_data_get_or_create(op_context, data_meta, pkg->head.originBodyLen);
        if (data == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: %s create data fail, capacity=%d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), dr_meta_name(data_meta), pkg->head.originBodyLen);
            return -1;
        }

        input_size = pkg->head.bodylen;
        output_size = logic_data_capacity(data);
        if (dr_cvt_decode(
                bpg_req_cvt(req),
                data_meta,
                logic_data_data(data), &output_size,
                pkg->body, &input_size,
                em, logic_context_flag_is_enable(op_context, logic_context_flag_debug)) != 0)
        {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: %s decode data fail, input len is %d, output len is %d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), 
                dr_meta_name(data_meta), pkg->head.bodylen, (int)logic_data_capacity(data));
            return -1;
        }
    }

    return 0;
}

static int bpg_rsp_copy_append_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t req, struct basepkg * pkg, error_monitor_t em) {
    LPDRMETALIB metalib;
    LPDRMETA data_meta;
    logic_data_t data;
    int i;
    int buf_begin;
    bpg_manage_t mgr;

    mgr = rsp->m_mgr;
    assert(mgr);

    metalib = bpg_manage_metalib(mgr);
    assert(metalib); /*checked in bpg_rsp_copy_pkg_to_ctx*/

    if (pkg->head.appendInfoCount > (sizeof(pkg->head.appendInfos) / sizeof(pkg->head.appendInfos[0]))) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append info count %d overflow!",
            bpg_manage_name(mgr), bpg_rsp_name(rsp), pkg->head.appendInfoCount);
        return -1;
    }

    buf_begin =  pkg->head.bodylen;
    for(i = 0; i < pkg->head.appendInfoCount; ++i) {
        struct AppendInfo * append = &pkg->head.appendInfos[i];
        int buf_end = buf_begin + append->size;
        size_t input_size;
        size_t output_size;

        if (buf_end > pkg->head.bodytotallen) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: buf overflow, total len is %d, curent is %d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), i, pkg->head.bodytotallen, buf_end);
            return -1;
        }

        data_meta = dr_lib_find_meta_by_id(metalib, append->id);
        if (data_meta == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: meta of id %d not exist in lib!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), i, append->id);
            return -1;
        }

        data = logic_data_get_or_create(op_context, data_meta, append->originSize);
        if (data == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s create data fail, capacity=%d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), i, dr_meta_name(data_meta), append->originSize);
            return -1;
        }

        input_size = append->size;
        output_size = logic_data_capacity(data);
        if (dr_cvt_decode(
                bpg_req_cvt(req),
                data_meta,
                logic_data_data(data), &output_size,
                pkg->body, &input_size, em, logic_context_flag_is_enable(op_context, logic_context_flag_debug)) != 0)
        {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s decode data fail, input len is %d, output len is %d!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), i, dr_meta_name(data_meta), append->size, (int)logic_data_capacity(data));
            return -1;
        }

        buf_begin = buf_end;
    }

    return 0;
}

int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t req, struct basepkg * pkg, error_monitor_t em) {
    bpg_manage_t mgr;

    mgr = rsp->m_mgr;

    if (bpg_manage_metalib(mgr) == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: metalib %s not exist!",
            bpg_manage_name(mgr), bpg_rsp_name(rsp), 
            bpg_manage_metalib_name(mgr));
        return -1;
    }

    if (mgr->m_request_meta == NULL) {
        bpg_manage_request_meta(mgr);
        if (mgr->m_request_meta == NULL) {
            CPE_ERROR(
                em, "%s.%s: bpg_rsp_execute: copy_pkg_to_ctx: request meta %s not exist in metalib %s!",
                bpg_manage_name(mgr), bpg_rsp_name(rsp), 
                bpg_manage_request_meta_name(mgr), bpg_manage_metalib_name(mgr));
            return -1;
        }    
    }

    if (bpg_rsp_copy_main_to_ctx(rsp, op_context, req, pkg, em) != 0) return -1;
    if (bpg_rsp_copy_append_to_ctx(rsp, op_context, req, pkg, em) != 0) return -1;

    return 0;
}

int bpg_rsp_copy_req_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t bpg_req, error_monitor_t em) {
    LPDRMETA carry_meta;
    logic_data_t data;
    bpg_manage_t mgr;
    mgr = rsp->m_mgr;

    carry_meta = bpg_req_carry_data_meta(bpg_req);
    if (carry_meta == NULL) return 0;

    data = logic_data_get_or_create(op_context, carry_meta, bpg_req_carry_data_size(bpg_req));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_carry_data: %s create data fail, size=%d!",
            bpg_manage_name(mgr), bpg_rsp_name(rsp),
            dr_meta_name(carry_meta), (int)bpg_req_carry_data_size(bpg_req));
        return -1;
    }

    memcpy(logic_data_data(data), bpg_req_carry_data(bpg_req), bpg_req_carry_data_size(bpg_req));
    return 0;
}

int bpg_rsp_copy_bpg_carry_data_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, bpg_req_t bpg_req, struct basepkg * pkg, error_monitor_t em) {
    LPDRMETA bpg_carry_data_meta;
    logic_data_t data;
    struct bpg_carry_info * buf;
    bpg_manage_t mgr;

    mgr = rsp->m_mgr;

    bpg_carry_data_meta =dr_lib_find_meta_by_name(bpg_metalib(rsp->m_mgr), "bpg_carry_info");
    if (bpg_carry_data_meta == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_bpg_carry_data: bpg_carry_info meta not exist!",
            bpg_manage_name(mgr), bpg_rsp_name(rsp));
        return -1;
    }

    data = logic_data_get_or_create(op_context, bpg_carry_data_meta, dr_meta_size(bpg_carry_data_meta));
    if (data == NULL) {
        CPE_ERROR(
            em, "%s.%s: bpg_rsp_execute: copy_bpg_carry_data: %s create data fail, size=%d!",
            bpg_manage_name(mgr), bpg_rsp_name(rsp),
            dr_meta_name(bpg_carry_data_meta), (int)dr_meta_size(bpg_carry_data_meta));
        return -1;
    }

    buf = (struct bpg_carry_info *)logic_data_data(data);
    buf->clientId = pkg->head.clientId;
    buf->cmd = pkg->head.cmd;
    buf->carry_data_size = bpg_req_carry_data_size(bpg_req);

    if (bpg_req_carry_data_meta(bpg_req)) {
        strncpy(buf->carry_meta_name, dr_meta_name(bpg_req_carry_data_meta(bpg_req)), sizeof(buf->carry_meta_name));
    }
    else {
        buf->carry_meta_name[0] = 0;
    }

    strncpy(buf->cvt_name, bpg_req_cvt_name(bpg_req), sizeof(buf->cvt_name));

    return 0;
}
