#include <assert.h>
#include "usf/room/room_manage.h"
#include "usf/room/room_user.h"
#include "room_internal_ops.h"

usf_room_user_t
usf_room_user_create(
    usf_room_manage_t mgr,
    usf_id_t role_id,
    size_t role_data_capacity)
{
    usf_room_user_t user;

    assert(mgr);

    user = (usf_room_user_t)mem_alloc(mgr->m_alloc, sizeof(struct usf_room_user) + role_data_capacity);
    if (user == NULL) return NULL;

    user->m_mgr = mgr;
    user->m_id = role_id;
    user->m_capacity = role_data_capacity;

    cpe_hash_entry_init(&user->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_users, user) != 0) {
        CPE_ERROR(
            mgr->m_em, "usf_room_user_create: user %d already exist in %s!", 
            (int)role_id, usf_room_manage_name(mgr));
        mem_free(mgr->m_alloc, user);
        return NULL;
    }

    return user;
}

void usf_room_user_free(usf_room_user_t user) {
    assert(user);
    assert(user->m_mgr);

    cpe_hash_table_remove_by_ins(&user->m_mgr->m_users, user);
    mem_free(user->m_mgr->m_alloc, user);
}

usf_id_t usf_room_user_id(usf_room_user_t user) {
    return user->m_id;
}

size_t usf_room_user_data_capacity(usf_room_user_t user) {
    return user->m_capacity;
}

void * usf_room_user_data(usf_room_user_t user) {
    return user + 1;
}

uint32_t usf_room_user_hash(const struct usf_room_user * user) {
    return usf_id_hash(user->m_id);
}

int usf_room_user_cmp(const struct usf_room_user * l, const struct usf_room_user * r) {
    return l->m_id == r->m_id;
}

void usf_room_user_free_all(usf_room_manage_t mgr) {
    struct cpe_hash_it user_it;
    usf_room_user_t user;

    cpe_hash_it_init(&user_it, &mgr->m_users);

    user = cpe_hash_it_next(&user_it);
    while(user) {
        usf_room_user_t next = cpe_hash_it_next(&user_it);
        usf_room_user_free(user);
        user = next;
    }
}
