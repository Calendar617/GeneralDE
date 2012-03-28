#ifndef USF_ROOM_ROOM_TYPE_H
#define USF_ROOM_ROOM_TYPE_H
#include "cpe/utils/memory.h"
#include "room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

usf_room_type_t
usf_room_type_create(usf_room_manage_t mgr, const char * name);

void usf_room_type_free(usf_room_type_t type);
const char * usf_room_type_name(usf_room_type_t type);

#ifdef __cplusplus
}
#endif

#endif
