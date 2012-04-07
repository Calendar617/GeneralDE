#ifndef USF_BPG_RSP_H
#define USF_BPG_RSP_H
#include "bpg_rsp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_rsp_t bpg_rsp_create(bpg_rsp_manage_t mgr, cfg_t cfg);
void bpg_rsp_free(bpg_rsp_t rsp);

const char * bpg_rsp_name(bpg_rsp_t rsp);
cpe_hash_string_t bpg_rsp_name_hs(bpg_rsp_t rsp);

uint32_t bpg_rsp_flags(bpg_rsp_t rsp);
void bpg_rsp_flags_set(bpg_rsp_t rsp, uint32_t flag);
void bpg_rsp_flag_enable(bpg_rsp_t rsp, bpg_rsp_flag_t flag);
void bpg_rsp_flag_disable(bpg_rsp_t rsp, bpg_rsp_flag_t flag);
int bpg_rsp_flag_is_enable(bpg_rsp_t rsp, bpg_rsp_flag_t flag);

int bpg_rsp_build(bpg_rsp_manage_t mgr, cfg_t cfg, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
