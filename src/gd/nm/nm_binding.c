#include <assert.h>
#include <string.h>
#include "nm_internal_ops.h"

void gd_nm_binding_free(struct gd_nm_binding * binding) {
    gd_nm_mgr_t nmm;
    assert(binding);
    assert(binding->m_group);
    assert(binding->m_node);
    assert(binding->m_group->m_mgr == binding->m_node->m_mgr);

    nmm = binding->m_node->m_mgr;

    TAILQ_REMOVE(
        &binding->m_node->m_to_group_bindings,
        binding,
        m_qh);

    cpe_hash_table_remove_by_ins(
        &binding->m_group->m_subs,
        binding);

    gd_nm_binding_put(nmm, binding);
}

struct gd_nm_binding *
gd_nm_binding_create(struct gd_nm_group * group, gd_nm_node_t node) {
    struct gd_nm_binding * binding;

    assert(group);
    assert(node);
    assert(group->m_mgr == node->m_mgr);

    binding = gd_nm_binding_get(group->m_mgr);
    if (binding == NULL) return NULL;

    binding->m_group = group;
    binding->m_node = node;

    if (cpe_hash_table_insert_unique(&group->m_subs, binding) != 0) {
        gd_nm_binding_put(group->m_mgr, binding);
        return NULL;
    }

    TAILQ_INSERT_HEAD(&node->m_to_group_bindings, binding, m_qh);

    return binding;
}

struct gd_nm_binding * gd_nm_binding_get(gd_nm_mgr_t nmm) {
    assert(nmm);
    if (TAILQ_EMPTY(&nmm->m_binding_free_list)) {
        return
            (struct gd_nm_binding *)
            mem_buffer_alloc(
                &nmm->m_binding_buffer,
                sizeof(struct gd_nm_binding));
    }
    else {
        struct gd_nm_binding * r =
            TAILQ_FIRST(&nmm->m_binding_free_list);
        TAILQ_REMOVE(&nmm->m_binding_free_list, r, m_qh);
        return r;
    }
}

void gd_nm_binding_put(gd_nm_mgr_t nmm, struct gd_nm_binding * binding) {
    TAILQ_INSERT_HEAD(&nmm->m_binding_free_list, binding, m_qh);
}

uint32_t gd_nm_binding_node_name_hash(const struct gd_nm_binding * binding) {
    return cpe_hs_value(binding->m_node->m_name);
}

int gd_nm_binding_node_name_cmp(const struct gd_nm_binding * l, const struct gd_nm_binding * r) {
    return cpe_hs_len(l->m_node->m_name) == cpe_hs_len(r->m_node->m_name)
        && strcmp(cpe_hs_data(l->m_node->m_name), cpe_hs_data(r->m_node->m_name)) == 0;
}
