#ifndef USF_BPG_INTERNAL_OPS_H
#define USF_BPG_INTERNAL_OPS_H
#include "gd/dp/dp_types.h"
#include "bpg_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int bpg_rsp_execute(gd_dp_req_t req, void * ctx, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
