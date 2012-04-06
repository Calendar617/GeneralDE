#ifndef GD_APP_MODULE_H
#define GD_APP_MODULE_H
#include "cpe/utils/hash_string.h"
#include "app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

const char * gd_app_module_name(gd_app_module_t module);
const char * gd_app_module_type_name(gd_app_module_t module);
nm_node_t gd_app_module_data(gd_app_context_t context, const char * moduleName);
gd_app_lib_t gd_app_module_lib(gd_app_module_t module);

gd_app_module_t
gd_app_install_module(
    gd_app_context_t context,
    const char * name,
    const char * type,
    const char * libName,
    cfg_t moduleCfg);

gd_app_module_t
gd_app_find_module(
    gd_app_context_t context,
    const char * name);

int gd_app_uninstall_module(
    gd_app_context_t context,
    const char * name);

int gd_app_bulk_install_module(
    gd_app_context_t context,
    gd_app_module_def_t module_defs,
    int module_def_count,
    void * ctx);

#ifdef __cplusplus
}
#endif

#endif