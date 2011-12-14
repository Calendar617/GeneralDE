#include <assert.h>
#include <string.h>
#include "gd/nm/nm_manage.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_node_alloc(
    gd_nm_mgr_t nmm,
    cpe_hash_string_t name,
    gd_nm_node_type_t type,
    size_t bodyLen,
    size_t capacity)
{
    char * buf;
    gd_nm_node_t node;

    assert(nmm);
    assert(name);

    buf = mem_alloc(nmm->m_alloc, cpe_hs_binary_len(name) + bodyLen + capacity);
    if (buf == NULL) return NULL;

    cpe_hs_copy((cpe_hash_string_t)buf, name);

    node = (gd_nm_node_t)
        (buf
         + cpe_hs_binary_len(name));

    node->m_mgr = nmm;
    node->m_type = type;
    node->m_name = (cpe_hash_string_t)buf;
    node->m_data_capacity = capacity;
    TAILQ_INIT(&node->m_to_group_bindings);
    cpe_hash_entry_init(&node->m_hh_for_mgr);

    return node;
}

void gd_nm_node_free(gd_nm_node_t node) {
    if (node == NULL) return;
    cpe_hash_table_remove_by_ins(&node->m_mgr->m_nodes, node);
}

void gd_nm_node_free_from_mgr(gd_nm_node_t node) {
    if (node->m_type == gd_nm_node_group) {
        gd_nm_group_free_from_mgr((struct gd_nm_group *)node);
    }
    else {
        gd_nm_instance_free_from_mgr((struct gd_nm_instance *)node);
    }
}

void gd_nm_node_free_from_mgr_base(gd_nm_node_t node) {
    assert(node);

    while(!TAILQ_EMPTY(&node->m_to_group_bindings)) {
        gd_nm_binding_free(
            TAILQ_FIRST(&node->m_to_group_bindings));
    }

    mem_free(node->m_mgr->m_alloc, node->m_name);
}

size_t gd_nm_node_capacity(gd_nm_node_t node) {
    return node->m_data_capacity;
}

void * gd_nm_node_data(gd_nm_node_t node) {
    return node->m_type == gd_nm_node_group
        ? (void*)(((struct gd_nm_group *)node) + 1)
        : (void*)(((struct gd_nm_instance *)node) + 1);
}

uint32_t gd_nm_node_hash(const gd_nm_node_t node) {
    return cpe_hs_value(node->m_name);
}

int gd_nm_node_cmp(const gd_nm_node_t l, const gd_nm_node_t r) {
    return cpe_hs_value(l->m_name) == cpe_hs_value(r->m_name)
        && strcmp(cpe_hs_data(l->m_name), cpe_hs_data(r->m_name)) == 0;
}

const char * gd_nm_node_name(gd_nm_node_t node) {
    return cpe_hs_data(node->m_name);
}

cpe_hash_string_t
gd_nm_node_hs_name(gd_nm_node_t node) {
    return node->m_name;
}

gd_nm_node_type_t gd_nm_node_type(gd_nm_node_t node) {
    return node->m_type;
}

gd_nm_node_t gd_nm_node_next_group(gd_nm_node_it_t it) {
    struct gd_nm_node_gruops_it * groupIt;
    gd_nm_node_t rv;

    assert(it);
    groupIt = (struct gd_nm_node_gruops_it *)it;

    if (groupIt->m_curent == NULL) return NULL;

    rv = (gd_nm_node_t)groupIt->m_curent->m_group;
    groupIt->m_curent = TAILQ_NEXT(groupIt->m_curent, m_qh);
    return rv;
}

void gd_nm_node_groups(gd_nm_node_it_t it, gd_nm_node_t node) {
    struct gd_nm_node_gruops_it * groupIt;

    assert(it);
    assert(node);

    groupIt = (struct gd_nm_node_gruops_it *)it;
    groupIt->m_next_fun = gd_nm_node_next_group;
    groupIt->m_curent = TAILQ_FIRST(&node->m_to_group_bindings);
}
