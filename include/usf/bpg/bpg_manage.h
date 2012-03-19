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
bpg_manage_find(gd_app_context_t app, cpe_hash_string_t name);

bpg_manage_t
bpg_manage_find_nc(gd_app_context_t app, const char * name);

bpg_manage_t
bpg_manage_default(gd_app_context_t app);

gd_app_context_t bpg_manage_app(bpg_manage_t mgr);
const char * bpg_manage_name(bpg_manage_t mgr);
cpe_hash_string_t bpg_manage_name_hs(bpg_manage_t mgr);

void bpg_manage_set_context_op(
    bpg_manage_t mgr,
    size_t ctx_capacity,
    bpg_logic_ctx_init_fun_t ctx_init,
    bpg_logic_ctx_fini_fun_t ctx_fini,
    void * ctx_ctx);

LPDRMETA bpg_manage_request_meta(bpg_manage_t mgr);
const char * bpg_manage_request_meta_name(bpg_manage_t mgr);
int bpg_manage_set_request_meta_name(bpg_manage_t mgr, const char * name);

LPDRMETA bpg_manage_response_meta(bpg_manage_t mgr);
const char * bpg_manage_response_meta_name(bpg_manage_t mgr);
int bpg_manage_set_response_meta_name(bpg_manage_t mgr, const char * name);

const char * bpg_manage_send_to(bpg_manage_t mgr);
int bpg_manage_set_send_to(bpg_manage_t mgr, const char * name);

const char * bpg_manage_metalib_name(bpg_manage_t mgr);
LPDRMETALIB bpg_manage_metalib(bpg_manage_t mgr);
int bpg_manage_set_metalib(bpg_manage_t mgr, const char * metalib_name);

uint32_t bpg_manage_flags(bpg_manage_t mgr);
void bpg_manage_flags_set(bpg_manage_t mgr, uint32_t flag);
void bpg_manage_flag_enable(bpg_manage_t mgr, bpg_manage_flag_t flag);
void bpg_manage_flag_disable(bpg_manage_t mgr, bpg_manage_flag_t flag);
int bpg_manage_flag_is_enable(bpg_manage_t mgr, bpg_manage_flag_t flag);

LPDRMETALIB bpg_metalib(bpg_manage_t mgr);
LPDRMETA bpg_meta_pkghead(bpg_manage_t mgr);
LPDRMETA bpg_meta_pkg(bpg_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
