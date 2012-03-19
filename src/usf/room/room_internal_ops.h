#ifndef USF_ROOM_INTERNAL_OPS_H
#define USF_ROOM_INTERNAL_OPS_H
#include "room_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*monitor ops*/
uint32_t usf_room_monitor_hash(const struct usf_room_monitor * monitor);
int usf_room_monitor_cmp(const struct usf_room_monitor * l, const struct usf_room_monitor * r);
void usf_room_monitor_free_all(usf_room_manage_t mgr);

/*room type ops*/
uint32_t usf_room_type_hash(const struct usf_room_type * type);
int usf_room_type_cmp(const struct usf_room_type * l, const struct usf_room_type * r);
void usf_room_type_free_all(usf_room_manage_t mgr);

/*room ops*/
uint32_t usf_room_hash(const struct usf_room * room);
int usf_room_cmp(const struct usf_room * l, const struct usf_room * r);
void usf_room_free_all(usf_room_manage_t mgr);

/*room user ops*/
uint32_t usf_room_user_hash(const struct usf_room_user * user);
int usf_room_user_cmp(const struct usf_room_user * l, const struct usf_room_user * r);
void usf_room_user_free_all(usf_room_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
