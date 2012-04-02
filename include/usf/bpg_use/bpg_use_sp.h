#ifndef USF_BPG_USE_SP_H
#define USF_BPG_USE_SP_H
#include "cpe/cfg/cfg_types.h"
#include "bpg_use_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_use_sp_t
bpg_use_sp_create(
    gd_app_context_t app,
    cfg_t cfg,
    error_monitor_t em);

void bpg_use_sp_free(bpg_use_sp_t sp);

gd_app_context_t bpg_use_sp_app(bpg_use_sp_t sp);
const char * bpg_use_sp_name(bpg_use_sp_t sp);

int bpg_use_sp_send(bpg_use_sp_t sp, bpg_pkg_t pkg);

size_t bpg_use_sp_buf_capacity(bpg_use_sp_t sp);
bpg_pkg_t bpg_use_sp_pkg_buf(bpg_use_sp_t sp);
void * bpg_use_sp_data_buf(bpg_use_sp_t sp);

bpg_pkg_manage_t bpg_use_sp_pkg_manage(bpg_use_sp_t sp);
LPDRMETALIB bpg_use_sp_metalib(bpg_use_sp_t sp);
LPDRMETA bpg_use_sp_meta(bpg_use_sp_t sp, const char * name);

#ifdef __cplusplus
}
#endif

#endif
