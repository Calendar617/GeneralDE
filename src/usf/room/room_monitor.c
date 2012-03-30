#include <assert.h>
#include "usf/room/room_manage.h"
#include "usf/room/room_monitor.h"
#include "room_internal_ops.h"

usf_room_monitor_t
usf_room_monitor_create(usf_room_manage_t mgr, const char * name) {
    usf_room_monitor_t monitor;
    size_t name_len;

    assert(mgr);

    name_len = strlen(name) + 1;

    monitor = (usf_room_monitor_t)mem_alloc(mgr->m_alloc, sizeof(struct usf_room_monitor) + name_len);
    if (monitor == NULL) return NULL;

    monitor->m_mgr = mgr;
    monitor->m_name = (char *)(monitor + 1);

    memcpy(monitor->m_name, name, name_len);

    cpe_hash_entry_init(&monitor->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_monitors, monitor) != 0) {
        CPE_ERROR(
            mgr->m_em, "usf_room_monitor_create: monitor %s already exist in %s!", 
            name, usf_room_manage_name(mgr));
        mem_free(mgr->m_alloc, monitor);
        return NULL;
    }

    return monitor;
}

void usf_room_monitor_free(usf_room_monitor_t monitor) {
    assert(monitor);
    assert(monitor->m_mgr);

    cpe_hash_table_remove_by_ins(&monitor->m_mgr->m_monitors, monitor);
    mem_free(monitor->m_mgr->m_alloc, monitor);
}

uint32_t usf_room_monitor_hash(const struct usf_room_monitor * monitor) {
    return cpe_hash_str(monitor->m_name, strlen(monitor->m_name));
}

int usf_room_monitor_cmp(const struct usf_room_monitor * l, const struct usf_room_monitor * r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void usf_room_monitor_free_all(usf_room_manage_t mgr) {
    struct cpe_hash_it monitor_it;
    usf_room_monitor_t monitor;

    cpe_hash_it_init(&monitor_it, &mgr->m_monitors);

    monitor = cpe_hash_it_next(&monitor_it);
    while(monitor) {
        usf_room_monitor_t next = cpe_hash_it_next(&monitor_it);
        usf_room_monitor_free(monitor);
        monitor = next;
    }
}
