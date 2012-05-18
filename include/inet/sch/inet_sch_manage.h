#ifndef INET_SCH_MANAGE_H
#define INET_SCH_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "gd/app/app_types.h"
#include "inet_sch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

inet_sch_manage_t
inet_sch_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc,
    error_monitor_t em);

void inet_sch_manage_free(inet_sch_manage_t mgr);

inet_sch_manage_t
inet_sch_manage_find(gd_app_context_t app, cpe_hash_string_t name);

inet_sch_manage_t
inet_sch_manage_find_nc(gd_app_context_t app, const char * name);

inet_sch_manage_t
inet_sch_manage_default(gd_app_context_t app);

gd_app_context_t inet_sch_manage_app(inet_sch_manage_t mgr);
const char * inet_sch_manage_name(inet_sch_manage_t mgr);
cpe_hash_string_t inet_sch_manage_name_hs(inet_sch_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif

