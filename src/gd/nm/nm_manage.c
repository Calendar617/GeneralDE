#include <assert.h>
#include "gd/nm/nm_manage.h"
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

    cpe_hash_table_set_destory_fun(
        &nmm->m_nodes,
        (cpe_hash_destory_t)gd_nm_node_free_from_mgr,
        NULL);

    mem_buffer_init(&nmm->m_binding_buffer, alloc);
    TAILQ_INIT(&nmm->m_binding_free_list);

    return nmm;
}

void gd_nm_mgr_free(gd_nm_mgr_t nmm) {
    if (nmm == NULL) return;

    cpe_hash_table_fini(&nmm->m_nodes);

    mem_buffer_clear(&nmm->m_binding_buffer);

    mem_free(nmm->m_alloc, nmm);
}

gd_nm_node_t
gd_nm_group_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity) {
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
            &group->m_subs,
            nmm->m_alloc,
            (cpe_hash_fun_t)gd_nm_binding_node_name_hash,
            (cpe_hash_cmp_t)gd_nm_binding_node_name_cmp,
            CPE_HASH_OBJ2ENTRY(gd_nm_binding, m_hh_for_group),
            0) != 0)
    {
        gd_nm_node_free_from_mgr((gd_nm_node_t)group);
        return NULL;
    }

    if (cpe_hash_table_insert_unique(&nmm->m_nodes, group) != 0) {
        gd_nm_group_free_from_mgr(group);
        return NULL;
    }
    
    return (gd_nm_node_t)group;
}

gd_nm_node_t
gd_nm_instance_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity) {
    struct gd_nm_instance * instance;

    assert(nmm);
    assert(name);

    instance = (struct gd_nm_instance *)
        gd_nm_node_alloc(
            nmm, name,
            gd_nm_node_instance, sizeof(struct gd_nm_instance),
            capacity);
    if (instance == NULL) return NULL;

    if (cpe_hash_table_insert_unique(&nmm->m_nodes, instance) != 0) {
        gd_nm_instance_free_from_mgr(instance);
        return NULL;
    }

    return (gd_nm_node_t)instance;
}

void gd_nm_group_free_from_mgr(struct gd_nm_group * group) {
    cpe_hash_table_fini(&group->m_subs);
    gd_nm_node_free_from_mgr((gd_nm_node_t)group);
}
