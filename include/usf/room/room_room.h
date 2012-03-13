#ifndef USF_ROOM_ROOM_H
#define USF_ROOM_ROOM_H
#include "room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

usf_room_t
usf_room_create(usf_room_manage_t mgr, usf_room_type_t type);

void usf_room_free(usf_room_t room);

int usf_room_add_user(usf_room_t room, usf_room_user_t user);
int usf_room_remove_user(usf_room_t room, usf_room_user_t user);

#ifdef __cplusplus
}
#endif

#endif
