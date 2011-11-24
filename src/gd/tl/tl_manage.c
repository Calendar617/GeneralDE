#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/tl/tl_errno.h"
#include "tl_internal_types.h"

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc) {
    return NULL;
}

void gd_tl_manage_free(gd_tl_manage_t tm) {
    if (tm == NULL) return;
}

struct gd_tl_event_node *
gd_tl_event_node_alloc(gd_tl_t tl, size_t capacity) {
    struct gd_tl_event_node * node = 
        (struct gd_tl_event_node *)mem_alloc(
            tl->m_manage->m_alloc,
            sizeof(struct gd_tl_event_node) + capacity);
    if (node == NULL) return NULL;

    node->m_event.m_tl = tl;
    node->m_event.m_capacity = capacity;
    
    return node;
}

void gd_tl_event_node_free(struct gd_tl_event_node * node) {
    if (node == NULL) return;

    mem_free(node->m_event.m_tl->m_manage->m_alloc, node);
}

int gd_tl_event_node_insert(struct gd_tl_event_node * node) {
    return 0;
}
