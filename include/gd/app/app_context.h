#ifndef GD_APP_CONTEXT_H
#define GD_APP_CONTEXT_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "app_basic.h"

#ifdef __cplusplus
extern "C" {
#endif

extern gd_app_context_t g_app_context;

gd_app_context_t gd_app_context_create(
    mem_allocrator_t alloc, size_t capacity,
    int argc, char ** argv);

void gd_app_context_free(gd_app_context_t context);

void gd_app_set_main(gd_app_context_t context, gd_app_main mf);

/*arg operations*/
int gd_app_argc(gd_app_context_t context);
char ** gd_app_argv(gd_app_context_t context);
int gd_app_add_arg(gd_app_context_t context, char * arg);

/*basic suupport operations*/
void gd_app_set_em(gd_app_context_t context, error_monitor_t em);
mem_allocrator_t gd_app_alloc(gd_app_context_t context);

/*config operations*/
int gd_app_cfg_reload(gd_app_context_t context);
cfg_t gd_app_cfg(gd_app_context_t context);

/*extern module operations*/
gd_tl_manage_t gd_app_tl_mgr(gd_app_context_t context);
gd_dp_mgr_t gd_app_dp_mgr(gd_app_context_t context);
gd_nm_mgr_t gd_app_nm_mgr(gd_app_context_t context);

/*app global infos*/
int gd_app_set_root(gd_app_context_t context, const char * root);
const char * gd_app_root(gd_app_context_t context);

void * gd_app_user_data(gd_app_context_t context);

void gd_app_set_state(gd_app_context_t context, gd_app_status_t state);
gd_app_status_t gd_app_state(gd_app_context_t context);

/*the main*/
int gd_app_run(gd_app_context_t context);

#ifdef __cplusplus
}
#endif

#endif
