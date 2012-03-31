#ifndef LOBBYSERVER_ROLE_MANAGE_H
#define LOBBYSERVER_ROLE_MANAGE_H
#include "cpe/utils/hash_string.h"
#include "dm_role_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dm_role_manage_t
dm_role_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em);

void dm_role_manage_free(dm_role_manage_t mgr);

dm_role_manage_t
dm_role_manage_find(gd_app_context_t app, cpe_hash_string_t name);

dm_role_manage_t
dm_role_manage_find_nc(gd_app_context_t app, const char * name);

dm_role_manage_t
dm_role_manage_default(gd_app_context_t app);

gd_app_context_t dm_role_manage_app(dm_role_manage_t mgr);
const char * dm_role_manage_name(dm_role_manage_t mgr);
cpe_hash_string_t dm_role_manage_name_hs(dm_role_manage_t mgr);


#ifdef __cplusplus
}
#endif

#endif
