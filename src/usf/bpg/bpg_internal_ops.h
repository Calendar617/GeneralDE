#ifndef USF_BPG_INTERNAL_OPS_H
#define USF_BPG_INTERNAL_OPS_H
#include "gd/dp/dp_types.h"
#include "bpg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*bpg_manage ops*/
bpg_req_t bpg_manage_rsp_buf(bpg_manage_t mgr, LPDRMETA carry_meta, size_t caary_capacity);

/*bpg_rsp ops*/
int bpg_rsp_execute(gd_dp_req_t req, void * ctx, error_monitor_t em);
void bpg_rsp_commit(logic_context_t op_context, void * user_data);

/*bpg_rsp_copy_info ops*/
struct bpg_rsp_copy_info * bpg_rsp_copy_info_create(bpg_manage_t mgr, const char * data_name);
void bpg_rsp_copy_info_free(bpg_manage_t mgr, struct bpg_rsp_copy_info * copy_info);

/*bpg_runing_req ops*/
uint32_t bpg_running_req_hash(const struct bpg_running_req * runing_req);
int bpg_running_req_cmp(const struct bpg_running_req * l, const struct bpg_running_req * r);
void bpg_running_req_free_all(bpg_manage_t mgr);

LPDRMETALIB bpg_metalib(bpg_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
