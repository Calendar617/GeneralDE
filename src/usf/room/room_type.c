#include <assert.h>
#include "usf/room/room_manage.h"
#include "usf/room/room_room_type.h"
#include "room_internal_ops.h"

usf_room_type_t
usf_room_type_create(usf_room_manage_t mgr, const char * name) {
    usf_room_type_t type;
    size_t name_len;

    assert(mgr);

    name_len = strlen(name) + 1;

    type = (usf_room_type_t)mem_alloc(mgr->m_alloc, sizeof(struct usf_room_type) + name_len);
    if (type == NULL) return NULL;

    type->m_mgr = mgr;
    type->m_name = (char *)(type + 1);

    memcpy(type->m_name, name, name_len);

    cpe_hash_entry_init(&type->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_room_types, type) != 0) {
        CPE_ERROR(
            mgr->m_em, "usf_room_type_create: type %s already exist in %s!", 
            name, usf_room_manage_name(mgr));
        mem_free(mgr->m_alloc, type);
        return NULL;
    }

    return type;
}

void usf_room_type_free(usf_room_type_t type) {
    assert(type);
    assert(type->m_mgr);

    cpe_hash_table_remove_by_ins(&type->m_mgr->m_room_types, type);
    mem_free(type->m_mgr->m_alloc, type);
}

uint32_t usf_room_type_hash(const struct usf_room_type * type) {
    return cpe_hash_str(type->m_name, strlen(type->m_name));
}

int usf_room_type_cmp(const struct usf_room_type * l, const struct usf_room_type * r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void usf_room_type_free_all(usf_room_manage_t mgr) {
    struct cpe_hash_it type_it;
    usf_room_type_t type;

    cpe_hash_it_init(&type_it, &mgr->m_room_types);

    type = cpe_hash_it_next(&type_it);
    while(type) {
        usf_room_type_t next = cpe_hash_it_next(&type_it);
        usf_room_type_free(type);
        type = next;
    }
}
