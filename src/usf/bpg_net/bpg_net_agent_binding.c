#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

int bpg_net_agent_binding_create(
    bpg_net_agent_t mgr,
    uint64_t client_id,
    int64_t connection_id)
{
    struct bpg_net_agent_binding * binding;

    binding = mem_alloc(mgr->m_alloc, sizeof(struct bpg_net_agent_binding));
    if (binding == NULL) return -1;

    cpe_hash_entry_init(&binding->m_hh_client);
    cpe_hash_entry_init(&binding->m_hh_connection);

    if (cpe_hash_table_insert_unique(&mgr->m_cliensts, binding) != 0) {
        CPE_ERROR(
            mgr->m_em, "%s: create client binding %d ==> %d: client is already exist!",
            bpg_net_agent_name(mgr), (int)client_id, (int)connection_id);
        mem_free(mgr->m_alloc, binding);
        return -1;
    }

    if (cpe_hash_table_insert_unique(&mgr->m_cliensts, binding) != 0) {
        CPE_ERROR(
            mgr->m_em, "%s: create client binding %d ==> %d: connection is already exist!",
            bpg_net_agent_name(mgr), (int)client_id, (int)connection_id);
        cpe_hash_table_remove_by_ins(&mgr->m_cliensts, binding);
        mem_free(mgr->m_alloc, binding);
        return -1;
    }

    return 0;
}

void bpg_net_agent_binding_free(bpg_net_agent_t mgr, struct bpg_net_agent_binding * binding) {
    cpe_hash_table_remove_by_ins(&mgr->m_cliensts, binding);
    cpe_hash_table_remove_by_ins(&mgr->m_connections, binding);
    mem_free(mgr->m_alloc, binding);
}

uint32_t bpg_net_agent_binding_client_id_hash(const struct bpg_net_agent_binding * binding) {
    return (uint32_t)binding->m_client_id;
}

int bpg_net_agent_binding_client_id_cmp(const struct bpg_net_agent_binding * l, const struct bpg_net_agent_binding * r) {
    return l->m_client_id == r->m_client_id;
}

uint32_t bpg_net_agent_binding_connection_id_hash(const struct bpg_net_agent_binding * binding) {
    return (uint32_t)binding->m_connection_id;
}

int bpg_net_agent_binding_connection_id_cmp(const struct bpg_net_agent_binding * l, const struct bpg_net_agent_binding * r) {
    return l->m_connection_id == r->m_connection_id;
}

void bpg_net_agent_binding_free_all(bpg_net_agent_t mgr) {
    struct cpe_hash_it binding_it;
    struct bpg_net_agent_binding * binding;

    cpe_hash_it_init(&binding_it, &mgr->m_cliensts);

    binding = cpe_hash_it_next(&binding_it);
    while(binding) {
        struct bpg_net_agent_binding * next = cpe_hash_it_next(&binding_it);
        bpg_net_agent_binding_free(mgr, binding);
        binding = next;
    }
}


