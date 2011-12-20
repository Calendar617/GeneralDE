#ifndef GD_APP_MODULE_H
#define GD_APP_MODULE_H
#include "cpe/utils/hash_string.h"
#include "app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

const char * gd_app_module_name(gd_app_module_t module);
cpe_hash_string_t gd_app_module_name_hs(gd_app_module_t module);
gd_nm_node_t gd_app_module_data(gd_app_context_t context, gd_app_module_t module);
gd_app_lib_t gd_app_module_lib(gd_app_module_t module);

gd_app_module_t
gd_app_install_module(
    gd_app_context_t context,
    const char * name,
    const char * libName,
    cfg_t moduleCfg);

#ifdef __cplusplus
}
#endif

#endif
