#ifndef GD_APP_INTERNAL_OPS_H
#define GD_APP_INTERNAL_OPS_H
#include "app_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*lib operations*/
struct gd_app_lib *
gd_app_lib_open_for_module(
    const char * libName, struct gd_app_module * module, error_monitor_t em);

void gd_app_lib_close_for_module(
    struct gd_app_lib * lib, struct gd_app_module * module, error_monitor_t em);

void * gd_app_lib_sym(struct gd_app_lib * lib, const char * symName);

/*module operations*/
int gd_app_modules_load(gd_app_context_t context);
void gd_app_modules_unload(gd_app_context_t context);

#ifdef __cplusplus
}
#endif

#endif

