#include <assert.h>
#include "gd/nm/nm_manage.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_group_create(gd_nm_mgr_t nmm, const char * name, size_t capacity) {
    struct gd_nm_group * group;

    assert(nmm);
    assert(name);

    group = (struct gd_nm_group *)
        gd_nm_node_alloc(
            nmm, name,
            gd_nm_node_group, sizeof(struct gd_nm_group),
            capacity);
    if (group == NULL) return NULL;

    if (cpe_hash_table_init(
            &group->m_members,
            nmm->m_alloc,
            (cpe_hash_fun_t)gd_nm_binding_node_name_hash,
            (cpe_hash_cmp_t)gd_nm_binding_node_name_cmp,
            CPE_HASH_OBJ2ENTRY(gd_nm_binding, m_hh_for_group),
            0) != 0)
    {
        gd_nm_node_free_from_mgr((gd_nm_node_t)group);
        return NULL;
    }

    cpe_hash_table_set_destory_fun(
        &group->m_members,
        (cpe_hash_destory_t)gd_nm_binding_free_from_group,
        NULL);

    if (cpe_hash_table_insert_unique(&nmm->m_nodes, group) != 0) {
        gd_nm_group_free_from_mgr(group);
        return NULL;
    }
    
    return (gd_nm_node_t)group;
}

void gd_nm_group_free_from_mgr(struct gd_nm_group * group) {
    cpe_hash_table_fini(&group->m_members);
    gd_nm_node_free_from_mgr_base((gd_nm_node_t)group);
}

int gd_nm_group_add_member(gd_nm_node_t grp, gd_nm_node_t sub) {
    if(grp == NULL
       || sub == NULL
       || grp->m_category != gd_nm_node_group)
    {
        return -1;
    }

    return (gd_nm_binding_create((struct gd_nm_group *)grp, sub) == NULL) ? -1 : 0;
}

gd_nm_node_t gd_nm_group_next_member(gd_nm_node_it_t it) {
    struct gd_nm_node_in_group_it * nodeIt;
    struct gd_nm_binding * binding;

    assert(it);
    nodeIt = (struct gd_nm_node_in_group_it *)it;

    binding = (struct gd_nm_binding *)cpe_hash_it_next(&nodeIt->m_hash_it);
    return binding ? binding->m_node : NULL;
}

int gd_nm_group_members(gd_nm_node_it_t it, gd_nm_node_t group) {
    struct gd_nm_node_in_group_it * nodeIt;

    assert(it);
    if (group == NULL
        || group->m_category != gd_nm_node_group) return -1;

    nodeIt = (struct gd_nm_node_in_group_it *)it;
    nodeIt->m_next_fun = gd_nm_group_next_member;
    cpe_hash_it_init(&nodeIt->m_hash_it, &((struct gd_nm_group *)group)->m_members);

    return 0;
}

void gd_nm_group_free_members(gd_nm_node_t node) {
    struct cpe_hash_it it;
    struct gd_nm_binding * binding;
    gd_nm_node_t preNode;

    assert(node->m_category == gd_nm_node_group);
    if (node->m_category != gd_nm_node_group) return;

    cpe_hash_it_init(&it, &((struct gd_nm_group *)node)->m_members);

    binding = (struct gd_nm_binding *)cpe_hash_it_next(&it);
    preNode = binding ? binding->m_node : NULL;

    while((binding = (struct gd_nm_binding *)cpe_hash_it_next(&it))) {
        gd_nm_node_free(preNode);
        preNode = binding->m_node;
    }

    if (preNode) {
        gd_nm_node_free(preNode);
    }
}

int gd_nm_group_member_count(gd_nm_node_t group) {
    if (group->m_category != gd_nm_node_group) {
        return 0;
    }
    else {
        return cpe_hash_table_count(&((struct gd_nm_group *)group)->m_members);
    }
}

gd_nm_node_t gd_nm_group_find_member(gd_nm_node_t group, cpe_hash_string_t name) {
    struct gd_nm_binding * binding;
    struct gd_nm_node buf;

    if (group->m_category != gd_nm_node_group) {
        return NULL;
    }

    buf.m_name = name;

    binding = (struct gd_nm_binding *)
        cpe_hash_table_find(&((struct gd_nm_group *)group)->m_members, &buf);

    return binding == NULL ? NULL : binding->m_node;
}
