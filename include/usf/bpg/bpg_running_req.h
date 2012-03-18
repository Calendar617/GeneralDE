#ifndef USF_BPG_RUNING_REQ_H
#define USF_BPG_RUNING_REQ_H
#include "cpe/dr/dr_types.h"
#include "bpg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_running_req_t
bpg_running_req_create(
    bpg_manage_t mgr,
    bpg_req_t req);

void bpg_running_req_free(bpg_running_req_t running_req);

bpg_req_sn_t bpg_running_req_sn(bpg_running_req_t running_req);

void bpg_running_req_set_response_to_str(bpg_running_req_t running_req, const char * response_to);
const char *  bpg_running_req_response_to_str(bpg_running_req_t running_req);

void bpg_running_req_set_response_to_int(bpg_running_req_t running_req, int32_t response_to);
int32_t  bpg_running_req_response_to_int(bpg_running_req_t running_req);

void bpg_running_req_set_logic_request(bpg_running_req_t running_req, logic_require_id_t logic_req_id);
logic_require_id_t bpg_running_req_logic_request(bpg_running_req_t running_req);

#ifdef __cplusplus
}
#endif

#endif
