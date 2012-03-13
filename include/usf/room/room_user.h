#ifndef USF_ROOM_USER_H
#define USF_ROOM_USER_H
#include "cpe/utils/memory.h"
#include "room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

usf_room_user_t
usf_room_user_create(
    usf_room_manage_t mgr,
    usf_id_t role_id,
    size_t role_data_capacity);

void usf_room_user_free(usf_room_user_t user);

usf_id_t usf_room_user_id(usf_room_user_t user);
size_t usf_room_user_data_capacity(usf_room_user_t user);
void * usf_room_user_data(usf_room_user_t user);

#ifdef __cplusplus
}
#endif

#endif
