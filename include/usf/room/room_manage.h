#ifndef USF_ROOM_MANAGE_H
#define USF_ROOM_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

usf_room_manage_t
usf_room_manage_create(
    gd_app_context_t app,
    const char * name,
    mem_allocrator_t alloc);

void usf_room_manage_free(usf_room_manage_t mgr);

usf_room_manage_t
usf_room_manage_find(gd_app_context_t app, cpe_hash_string_t name);

usf_room_manage_t
usf_room_manage_find_nc(gd_app_context_t app, const char * name);

usf_room_manage_t
usf_room_manage_default(gd_app_context_t app);

gd_app_context_t usf_room_manage_app(usf_room_manage_t mgr);
const char * usf_room_manage_name(usf_room_manage_t mgr);
cpe_hash_string_t usf_room_manage_name_hs(usf_room_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
