#ifndef USF_ROOM_TYPES_H
#define USF_ROOM_TYPES_H
#include "cpe/pal/pal_types.h"
#include "gd/app/app_types.h"
#include "usf/utils/utils_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct usf_room * usf_room_t;
typedef struct usf_room_type * usf_room_type_t;
typedef struct usf_room_user * usf_room_user_t;
typedef struct usf_room_manage * usf_room_manage_t;
typedef struct usf_room_monitor * usf_room_monitor_t;

typedef struct usf_room_it {
    usf_room_t (*next)(struct usf_room_it * it);
    char m_data[16];
} * usf_room_it_t;

typedef struct usf_room_user_it {
    usf_room_t (*next)(struct usf_room_user_it * it);
    char m_data[16];
} * usf_room_user_it_t;

#ifdef __cplusplus
}
#endif

#endif
