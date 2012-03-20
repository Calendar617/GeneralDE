#ifndef USF_BPG_RSP_INTERNAL_OPS_H
#define USF_BPG_RSP_INTERNAL_OPS_H
#include "gd/dp/dp_types.h"
#include "bpg_rsp_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*bpg_rsp_manage ops*/
bpg_pkg_t bpg_rsp_manage_rsp_buf(
    bpg_rsp_manage_t mgr,
    const char * pkg_mgr_name,
    LPDRMETA carry_meta, size_t caary_capacity);

/*bpg_rsp ops*/
int bpg_rsp_execute(gd_dp_req_t req, void * ctx, error_monitor_t em);
void bpg_rsp_commit(logic_context_t op_context, void * user_data);

/*bpg_rsp_copy_info ops*/
struct bpg_rsp_copy_info * bpg_rsp_copy_info_create(bpg_rsp_manage_t mgr, const char * data_name);
void bpg_rsp_copy_info_free(bpg_rsp_manage_t mgr, struct bpg_rsp_copy_info * copy_info);

#ifdef __cplusplus
}
#endif

#endif
