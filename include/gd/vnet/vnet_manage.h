#ifndef GD_VNET_MANAGE_H
#define GD_VNET_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "vnet_types.h"

#ifdef __cplusplus
extern "C" {
#endif

vnet_manage_t
vnet_manage_create(
    const char * name,
    gd_app_context_t app,
    mem_allocrator_t alloc);

void vnet_manage_free(vnet_manage_t mgr);

vnet_manage_t
vnet_manage_find(
    gd_app_context_t app,
    cpe_hash_string_t name);

vnet_manage_t
vnet_manage_find_nc(gd_app_context_t app, const char * name);

vnet_manage_t
vnet_manage_default(
    gd_app_context_t app);

gd_app_context_t vnet_manage_app(vnet_manage_t mgr);
const char * vnet_manage_name(vnet_manage_t mgr);
cpe_hash_string_t vnet_manage_name_hs(vnet_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
