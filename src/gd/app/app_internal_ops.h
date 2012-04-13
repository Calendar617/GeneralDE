#ifndef GD_APP_INTERNAL_OPS_H
#define GD_APP_INTERNAL_OPS_H
#include "app_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*tick operations*/
void gd_app_tick_chain_free(gd_app_context_t context);

/*tl operations*/
void gd_app_tl_free(struct gd_app_tl * app_tl);

/*lib operations*/
struct gd_app_lib *
gd_app_lib_open_for_module(
    const char * libName, struct gd_app_module_type * module, error_monitor_t em);

void gd_app_lib_close_for_module(
    struct gd_app_lib * lib, struct gd_app_module_type * module, error_monitor_t em);

void * gd_app_lib_sym(struct gd_app_lib * lib, const char * symName, error_monitor_t em);

/*module operations*/
int gd_app_modules_load(gd_app_context_t context);
void gd_app_modules_unload(gd_app_context_t context);

/*module type operations*/
struct gd_app_module_type *
gd_app_module_type_create(
    const char * type,
    const char * libName,
    error_monitor_t em);

void gd_app_module_type_free(struct gd_app_module_type * module, error_monitor_t em);

struct gd_app_module_type *
gd_app_module_type_find(const char * moduleName);

/*module data operations*/
nm_node_t
gd_app_module_data_load(
    gd_app_context_t context,
    const char * moduleName);

void gd_app_module_data_free(
    gd_app_context_t context,
    const char * moduleName);

#ifdef __cplusplus
}
#endif

#endif

