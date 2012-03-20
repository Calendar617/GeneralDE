#ifndef USF_BPG_PKG_MANAGE_H
#define USF_BPG_PKG_MANAGE_H
#include "cpe/utils/hash_string.h"
#include "cpe/dr/dr_types.h"
#include "bpg_pkg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bpg_pkg_manage_t
bpg_pkg_manage_create(
    gd_app_context_t app,
    const char * name,
    error_monitor_t em);

void bpg_pkg_manage_free(bpg_pkg_manage_t mgr);

bpg_pkg_manage_t
bpg_pkg_manage_find(gd_app_context_t app, cpe_hash_string_t name);

bpg_pkg_manage_t
bpg_pkg_manage_find_nc(gd_app_context_t app, const char * name);

bpg_pkg_manage_t
bpg_pkg_manage_default(gd_app_context_t app);

gd_app_context_t bpg_pkg_manage_app(bpg_pkg_manage_t mgr);
const char * bpg_pkg_manage_name(bpg_pkg_manage_t mgr);
cpe_hash_string_t bpg_pkg_manage_name_hs(bpg_pkg_manage_t mgr);

int bpg_pkg_manage_set_data_cvt(bpg_pkg_manage_t pkg, const char * cvt_name);
const char * bpg_pkg_manage_data_cvt_name(bpg_pkg_manage_t pkg);
dr_cvt_t bpg_pkg_manage_data_cvt(bpg_pkg_manage_t pkg);

int bpg_pkg_manage_set_base_cvt(bpg_pkg_manage_t pkg, const char * cvt_name);
const char * bpg_pkg_manage_base_cvt_name(bpg_pkg_manage_t pkg);
dr_cvt_t bpg_pkg_manage_base_cvt(bpg_pkg_manage_t pkg);

int bpg_pkg_manage_set_data_metalib(bpg_pkg_manage_t mgr, const char * metalib_name);
int bpg_pkg_manage_set_cmd_meta_name(bpg_pkg_manage_t mgr, const char * name);
LPDRMETA bpg_pkg_manage_cmd_meta(bpg_pkg_manage_t mgr);
const char * bpg_pkg_manage_cmd_meta_name(bpg_pkg_manage_t mgr);

const char * bpg_pkg_manage_data_metalib_name(bpg_pkg_manage_t mgr);
LPDRMETALIB bpg_pkg_manage_data_metalib(bpg_pkg_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
