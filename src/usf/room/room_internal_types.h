#ifndef USF_ROOM_INTERNAL_TYPES_H
#define USF_ROOM_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "cpe/utils/hash.h"
#include "cpe/utils/hash_string.h"
#include "usf/room/room_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct usf_room_manage {
    mem_allocrator_t m_alloc;
    gd_app_context_t m_app;
    error_monitor_t m_em;

    struct cpe_hash_table m_room_types;
    struct cpe_hash_table m_users;
    struct cpe_hash_table m_rooms;
    struct cpe_hash_table m_monitors;
};

struct usf_room {
    usf_room_manage_t m_mgr;
    usf_id_t m_id;
    usf_room_type_t m_type;

    struct cpe_hash_entry m_hh;
};

struct usf_room_user {
    usf_room_manage_t m_mgr;
    usf_id_t m_id;
    size_t m_capacity;

    struct cpe_hash_entry m_hh;
};

struct usf_room_type {
    usf_room_manage_t m_mgr;
    char * m_name;
    size_t m_data_capacity;
    struct cpe_hash_entry m_hh;
};

struct usf_room_monitor {
    usf_room_manage_t m_mgr;
    char * m_name;

    struct cpe_hash_entry m_hh;
};

#ifdef __cplusplus
}
#endif

#endif
