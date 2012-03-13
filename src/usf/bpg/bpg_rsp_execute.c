#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "gd/dp/dp_request.h"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_data.h"
#include "usf/bpg/bpg_manage.h"
#include "protocol/base/base_package.h"
#include "bpg_internal_ops.h"

extern LPDRMETALIB g_metalib_base_package;

static int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, struct basepkg * pkg, error_monitor_t em);
static int bpg_rsp_copy_ctx_to_pkg(bpg_rsp_t rsp, logic_context_t op_context, struct basepkg * pkg, size_t capacity, error_monitor_t em);

void bpg_rsp_commit(logic_context_t op_context, void * user_data) {
    struct basepkg * pkg;
    bpg_rsp_t bpg_rsp;
    bpg_manage_t bpg_mgr;
    gd_dp_req_t response_buf;
    error_monitor_t em;

    bpg_rsp = (bpg_rsp_t)user_data;
    assert(bpg_rsp);

    bpg_mgr = bpg_rsp->m_mgr;
    assert(bpg_mgr);

    em = bpg_mgr->m_em;

    response_buf = bpg_manage_rsp_buf(bpg_mgr);
    if (response_buf == NULL) {
        CPE_ERROR(em, "bpg_rsp_commit: response buf is NULL!");
        return;
    }

    if (gd_dp_req_capacity(response_buf) < sizeof(struct basepkg_head)) {
        CPE_ERROR(
            em, "bpg_rsp_commit: response buf size is too small, head size is %d, but response buf size is %d!",
            (int)sizeof(struct basepkg_head), (int)gd_dp_req_capacity(response_buf));
        return;
    }

    pkg = (struct basepkg *)gd_dp_req_data(response_buf);
    pkg->head.magic = BASEPKG_HEAD_MAGIC;
    pkg->head.version = 1;
    pkg->head.sn = logic_context_id(op_context);
    pkg->head.targetId = 0; //TODO
    pkg->head.errno = logic_context_errno(op_context);
    pkg->head.cmd = 0; //TODO
    pkg->head.flags = 0;
    pkg->head.headlen = 0;
    pkg->head.bodylen = 0;
    pkg->head.originBodyLen = 0;
    pkg->head.bodytotallen = 0;
    pkg->head.appendInfoCount = 0;

    if (bpg_rsp_copy_ctx_to_pkg(bpg_rsp, op_context, pkg, gd_dp_req_capacity(response_buf), em) != 0) {
    }
}

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

    if (bpg_rsp_copy_pkg_to_ctx(bpg_rsp, op_context, pkg, em) != 0) {
        CPE_ERROR(em, "bpg_rsp_execute: copy data from pdu to context!");
        logic_context_free(op_context);
        return 0;
    }

    if (logic_context_bind(op_context, bpg_rsp->m_executor) != 0) {
        CPE_ERROR(em, "bpg_rsp_execute: bind executor to context fail!");
        logic_context_free(op_context);
        return 0;
    }

    logic_context_set_commit(op_context, bpg_rsp_commit, bpg_rsp);

    logic_context_execute(op_context);

    return 0;
}

static int bpg_rsp_copy_main_to_ctx(bpg_manage_t mgr, logic_context_t op_context, struct basepkg * pkg, error_monitor_t em) {
    int cmd_entry_idx;
    LPDRMETA data_meta;
    logic_data_t data;

    cmd_entry_idx = dr_meta_find_entry_idx_by_id(mgr->m_request_meta, pkg->head.cmd);
    if (cmd_entry_idx < 0) {
        if (logic_context_flag_is_enable(op_context, logic_context_flag_debug)) {
            CPE_INFO(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: request meta %s have no associate info of cmd %d!",
                mgr->m_request_meta_name, pkg->head.cmd);
        }
    }
    else {
        data_meta = dr_entry_self_meta(dr_meta_entry_at(mgr->m_request_meta, cmd_entry_idx));
        if (data_meta == NULL) {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: %s.%s have no associate meta!",
                mgr->m_request_meta_name, dr_entry_name(dr_meta_entry_at(mgr->m_request_meta, cmd_entry_idx)));
            return -1;
        }

        data = logic_data_get_or_create(op_context, data_meta, pkg->head.originBodyLen);
        if (data == NULL) {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: %s create data fail, capacity=%d!",
                dr_meta_name(data_meta), pkg->head.originBodyLen);
            return -1;
        }

        if (mgr->m_cvt_decode(
                logic_data_data(data), logic_data_capacity(data),
                pkg->body, pkg->head.bodylen,
                mgr->m_cvt_ctx, em) != 0)
        {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: %s decode data fail, input len is %d, output len is %d!",
                dr_meta_name(data_meta), pkg->head.bodylen, (int)logic_data_capacity(data));
            return -1;
        }
    }

    return 0;
}

static int bpg_rsp_copy_append_to_ctx(bpg_manage_t mgr, logic_context_t op_context, struct basepkg * pkg, error_monitor_t em) {
    LPDRMETA data_meta;
    logic_data_t data;
    int i;
    int buf_begin;

    if (pkg->head.appendInfoCount > (sizeof(pkg->head.appendInfos) / sizeof(pkg->head.appendInfos[0]))) {
        CPE_ERROR(em, "bpg_rsp_execute: copy_pkg_to_ctx: append info count %d overflow!", pkg->head.appendInfoCount);
        return -1;
    }

    buf_begin =  pkg->head.bodylen;
    for(i = 0; i < pkg->head.appendInfoCount; ++i) {
        struct AppendInfo * append = &pkg->head.appendInfos[i];
        int buf_end = buf_begin + append->size;

        if (buf_end > pkg->head.bodytotallen) {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: append %d: buf overflow, total len is %d, curent is %d!",
                i, pkg->head.bodytotallen, buf_end);
            return -1;
        }

        data_meta = dr_lib_find_meta_by_id(mgr->m_metalib, append->id);
        if (data_meta == NULL) {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: append %d: meta of id %d not exist in lib!",
                i, append->id);
            return -1;
        }

        data = logic_data_get_or_create(op_context, data_meta, append->originSize);
        if (data == NULL) {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s create data fail, capacity=%d!",
                i, dr_meta_name(data_meta), append->originSize);
            return -1;
        }

        if (mgr->m_cvt_decode(
                logic_data_data(data), logic_data_capacity(data),
                pkg->body, append->size,
                mgr->m_cvt_ctx, em) != 0)
        {
            CPE_ERROR(
                em, "bpg_rsp_execute: copy_pkg_to_ctx: append %d: %s decode data fail, input len is %d, output len is %d!",
                i, dr_meta_name(data_meta), append->size, (int)logic_data_capacity(data));
            return -1;
        }

        buf_begin = buf_end;
    }

    return 0;
}

int bpg_rsp_copy_pkg_to_ctx(bpg_rsp_t rsp, logic_context_t op_context, struct basepkg * pkg, error_monitor_t em) {
    bpg_manage_t mgr;

    mgr = rsp->m_mgr;
    
    if (mgr->m_metalib == NULL) {
        CPE_ERROR(em, "bpg_rsp_execute: copy_pkg_to_ctx: metalib is not set!");
        return -1;
    }

    if (mgr->m_request_meta == NULL) {
        CPE_ERROR(em, "bpg_rsp_execute: copy_pkg_to_ctx: request meta is not set!");
        return -1;
    }

    if (mgr->m_cvt_decode == NULL) {
        CPE_ERROR(em, "bpg_rsp_execute: copy_pkg_to_ctx: decode is not set!");
        return -1;
    }

    if (bpg_rsp_copy_main_to_ctx(mgr, op_context, pkg, em) != 0) return -1;
    if (bpg_rsp_copy_append_to_ctx(mgr, op_context, pkg, em) != 0) return -1;

    return 0;
}

int bpg_rsp_copy_ctx_to_pkg(
    bpg_rsp_t rsp, logic_context_t op_context,
    struct basepkg * pkg, size_t data_capacity,
    error_monitor_t em)
{
    return 0;
}
