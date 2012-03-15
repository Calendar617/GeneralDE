#include <assert.h>
#include "usf/room/room_manage.h"
#include "usf/room/room_room.h"
#include "room_internal_ops.h"

usf_room_t
usf_room_create(
    usf_room_manage_t mgr,
    usf_room_type_t type)
{
    usf_room_t room;
    usf_id_t room_id;

    assert(mgr);
    assert(type);

    room_id = 0; //TODO

    room = (usf_room_t)mem_alloc(mgr->m_alloc, sizeof(struct usf_room) + type->m_data_capacity);
    if (room == NULL) return NULL;

    room->m_mgr = mgr;
    room->m_id = room_id;
    room->m_type = type;

    cpe_hash_entry_init(&room->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_rooms, room) != 0) {
        CPE_ERROR(
            mgr->m_em, "usf_room_create: room %d already exist in %s!", 
            (int)room_id, usf_room_manage_name(mgr));
        mem_free(mgr->m_alloc, room);
        return NULL;
    }

    return room;
}

void usf_room_free(usf_room_t room) {
    assert(room);
    assert(room->m_mgr);

    cpe_hash_table_remove_by_ins(&room->m_mgr->m_rooms, room);
    mem_free(room->m_mgr->m_alloc, room);
}

usf_id_t usf_room_id(usf_room_t room) {
    return room->m_id;
}

size_t usf_room_data_capacity(usf_room_t room) {
    return room->m_type->m_data_capacity;
}

void * usf_room_data(usf_room_t room) {
    return room + 1;
}

uint32_t usf_room_hash(const struct usf_room * room) {
    return usf_id_hash(room->m_id);
}

int usf_room_cmp(const struct usf_room * l, const struct usf_room * r) {
    return l->m_id == r->m_id;
}

void usf_room_free_all(usf_room_manage_t mgr) {
    struct cpe_hash_it room_it;
    usf_room_t room;

    cpe_hash_it_init(&room_it, &mgr->m_rooms);

    room = cpe_hash_it_next(&room_it);
    while(room) {
        usf_room_t next = cpe_hash_it_next(&room_it);
        usf_room_free(room);
        room = next;
    }
}
