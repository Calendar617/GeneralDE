#ifndef USF_BPG_MANAGE_H
#define USF_BPG_MANAGE_H
#include "bpg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_manage_t
bpg_manage_create(
    gd_app_context_t app,
    const char * name,
    logic_manage_t logic_mgr,
    error_monitor_t em);

void bpg_manage_free(bpg_manage_t mgr);

bpg_manage_t
bpg_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name);

bpg_manage_t
bpg_manage_default(
    gd_app_context_t app);

gd_app_context_t bpg_manage_app(bpg_manage_t mgr);
const char * bpg_manage_name(bpg_manage_t mgr);
cpe_hash_string_t bpg_manage_name_hs(bpg_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
