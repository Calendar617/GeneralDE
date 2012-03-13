#include <assert.h>
#include "cpe/pal/pal_stackbuf.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "nm_internal_ops.h"

gd_nm_mgr_t gd_nm_mgr_create(mem_allocrator_t alloc) {
    gd_nm_mgr_t nmm =
        (gd_nm_mgr_t)mem_alloc(
            alloc,
            sizeof(struct gd_nm_mgr));

    if (nmm == NULL) return NULL;

    nmm->m_alloc = alloc;
    if (cpe_hash_table_init(
            &nmm->m_nodes,
            alloc,
            (cpe_hash_fun_t)gd_nm_node_hash,
            (cpe_hash_cmp_t)gd_nm_node_cmp,
            CPE_HASH_OBJ2ENTRY(gd_nm_node, m_hh_for_mgr),
            0) != 0)
    {
        mem_free(alloc, nmm);
        return NULL;
    }

    mem_buffer_init(&nmm->m_binding_buffer, alloc);
    TAILQ_INIT(&nmm->m_binding_free_list);

    return nmm;
}

void gd_nm_mgr_free(gd_nm_mgr_t nmm) {
    struct cpe_hash_it node_it;
    gd_nm_node_t node;

    if (nmm == NULL) return;

    cpe_hash_it_init(&node_it, &nmm->m_nodes);
    node = cpe_hash_it_next(&node_it);
    while(node) {
        gd_nm_node_t next = cpe_hash_it_next(&node_it);
        gd_nm_node_free(node);
        node = next;
    }

    cpe_hash_table_fini(&nmm->m_nodes);

    mem_buffer_clear(&nmm->m_binding_buffer);

    mem_free(nmm->m_alloc, nmm);
}

gd_nm_node_t
gd_nm_mgr_find_node_nc(gd_nm_mgr_t nmm, const char * name) {
    size_t nameLen = cpe_hs_len_to_binary_len(strlen(name));
    char nameBuf[CPE_STACK_BUF_LEN(nameLen)];
    cpe_hs_init((cpe_hash_string_t)nameBuf, CPE_STACK_BUF_LEN(nameLen), name);
    return gd_nm_mgr_find_node(nmm, (cpe_hash_string_t)nameBuf);
}

gd_nm_node_t
gd_nm_mgr_find_node(gd_nm_mgr_t nmm, cpe_hash_string_t name) {
    struct gd_nm_node buf;

    buf.m_name = name;

    return (gd_nm_node_t)cpe_hash_table_find(&nmm->m_nodes, &buf);
}

gd_nm_node_t gd_nm_mgr_next_node(gd_nm_node_it_t it) {
    struct gd_nm_node_in_mgr_it * nodeIt;

    assert(it);
    nodeIt = (struct gd_nm_node_in_mgr_it *)it;

    return (gd_nm_node_t)cpe_hash_it_next(&nodeIt->m_hash_it);
}

int gd_nm_mgr_nodes(gd_nm_node_it_t it, gd_nm_mgr_t nmm) {
    struct gd_nm_node_in_mgr_it * nodeIt;

    assert(it);
    if (nmm == NULL) return -1;

    nodeIt = (struct gd_nm_node_in_mgr_it *)it;
    nodeIt->m_next_fun = gd_nm_mgr_next_node;
    cpe_hash_it_init(&nodeIt->m_hash_it, &nmm->m_nodes);

    return 0;
}

void gd_nm_mgr_free_nodes_with_type_name(gd_nm_mgr_t nmm, const char * type) {
    struct gd_nm_node_it it;
    gd_nm_node_t node;

    gd_nm_mgr_nodes(&it, nmm);

    while((node = gd_nm_node_next(&it))) {
        gd_nm_node_t next = gd_nm_node_next(&it);;

        if (strcmp(gd_nm_node_type_name(node), type) == 0) {
            gd_nm_node_free(node);
        }

        node = next;
    }
}

void gd_nm_mgr_free_nodes_with_type(gd_nm_mgr_t nmm, gd_nm_node_type_t type) {
    struct gd_nm_node_it it;
    gd_nm_node_t node;

    gd_nm_mgr_nodes(&it, nmm);

    while((node = gd_nm_node_next(&it))) {
        gd_nm_node_t next = gd_nm_node_next(&it);;

        if (node->m_type == type) {
            gd_nm_node_free(node);
        }

        node = next;
    }
}
