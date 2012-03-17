#include <assert.h>
#include "cpe/pal/pal_stackbuf.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_group_create(gd_nm_mgr_t nmm, const char * name, size_t capacity) {
    assert(nmm);
    assert(name);

    return gd_nm_node_alloc(
        nmm, name,
        gd_nm_node_group, sizeof(struct gd_nm_group),
        capacity);
}

int gd_nm_group_add_member(gd_nm_node_t grp, gd_nm_node_t sub) {
    if(grp == NULL
       || sub == NULL
       || grp->m_category != gd_nm_node_group)
    {
        return -1;
    }

    return (gd_nm_binding_create(gd_nm_group_from_node(grp), sub) == NULL) ? -1 : 0;
}

gd_nm_node_t gd_nm_group_next_member(gd_nm_node_it_t it) {
    struct gd_nm_node_in_group_it * nodeIt;
    struct gd_nm_binding * binding;

    assert(it);
    nodeIt = (struct gd_nm_node_in_group_it *)it;

    binding = (struct gd_nm_binding *)cpe_hash_it_next(&nodeIt->m_hash_it);
    return binding ? binding->m_node : NULL;
}

int gd_nm_group_members(gd_nm_node_it_t it, gd_nm_node_t node) {
    struct gd_nm_node_in_group_it * nodeIt;

    assert(it);
    if (node == NULL
        || node->m_category != gd_nm_node_group) return -1;

    nodeIt = (struct gd_nm_node_in_group_it *)it;
    nodeIt->m_next_fun = gd_nm_group_next_member;
    cpe_hash_it_init(&nodeIt->m_hash_it, &(gd_nm_group_from_node(node))->m_members);

    return 0;
}

void gd_nm_group_free_members(gd_nm_node_t node) {
    struct cpe_hash_it it;
    struct gd_nm_binding * binding;

    assert(node->m_category == gd_nm_node_group);
    if (node->m_category != gd_nm_node_group) return;

    cpe_hash_it_init(&it, &(gd_nm_group_from_node(node))->m_members);
    while((binding = (struct gd_nm_binding *)cpe_hash_it_next(&it))) {
        struct gd_nm_binding * next =  (struct gd_nm_binding *)cpe_hash_it_next(&it);
        gd_nm_node_free(binding->m_node);
        binding = next;
    }
}

int gd_nm_group_member_count(gd_nm_node_t node) {
    if (node->m_category != gd_nm_node_group) {
        return -1;
    }
    else {
        return cpe_hash_table_count(&(gd_nm_group_from_node(node))->m_members);
    }
}

gd_nm_node_t gd_nm_group_find_member_nc(gd_nm_node_t node, const char * name) {
    size_t nameLen = cpe_hs_len_to_binary_len(strlen(name));
    char buf[CPE_STACK_BUF_LEN(nameLen)];
    cpe_hs_init((cpe_hash_string_t)buf, sizeof(buf), name);
    return gd_nm_group_find_member(node, (cpe_hash_string_t)buf);
}

gd_nm_node_t gd_nm_group_find_member(gd_nm_node_t node, cpe_hash_string_t name) {
    struct gd_nm_binding * binding;
    struct gd_nm_node buf_node;
    struct gd_nm_binding buf_binding;

    if (node->m_category != gd_nm_node_group) {
        return NULL;
    }

    buf_binding.m_node = &buf_node;
    buf_node.m_name = name;

    binding = (struct gd_nm_binding *)
        cpe_hash_table_find(&(gd_nm_group_from_node(node))->m_members, &buf_binding);

    return binding == NULL ? NULL : binding->m_node;
}
