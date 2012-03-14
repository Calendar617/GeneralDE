#ifndef USF_ROOM_MONITOR_H
#define USF_ROOM_MONITOR_H
#include "room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

usf_room_monitor_t
usf_room_monitor_create(usf_room_manage_t mgr, const char * name);

void usf_room_monitor_free(usf_room_monitor_t monitor);
const char * usf_room_monitor_name(usf_room_monitor_t monitor);

#ifdef __cplusplus
}
#endif

#endif
