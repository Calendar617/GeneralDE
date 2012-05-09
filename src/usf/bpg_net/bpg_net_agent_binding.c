#include "cpe/net/net_endpoint.h"
#include "gd/app/app_context.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

bpg_net_pkg_next_step_t
bpg_net_agent_process_recv(bpg_net_agent_t agent, uint64_t client_id, uint32_t connection_id) {
    struct bpg_net_agent_binding * binding;

    if (connection_id == BPG_INVALID_CONNECTION_ID) {
        CPE_ERROR(
            agent->m_em, "%s: ep %d: binding: connection is invalid!",
            bpg_net_agent_name(agent), (int)connection_id);
        return bpg_net_pkg_next_close;
    }

    if (client_id == 0) return bpg_net_pkg_next_go_with_connection_id;

    binding = bpg_net_agent_binding_find_by_connection_id(agent, connection_id);
    if (binding == NULL) {
        CPE_ERROR(
            agent->m_em, "%s: ep %d: binding: no binding, reject client %d!",
            bpg_net_agent_name(agent), (int)connection_id, (int)client_id);
        return bpg_net_pkg_next_close;
    }

    if (binding->m_client_id != client_id) {
        CPE_ERROR(
            agent->m_em, "%s: ep %d: binding: connection already bind to client %d, now client is %d!",
            bpg_net_agent_name(agent), (int)connection_id, (int)binding->m_client_id, (int)client_id);
        return bpg_net_pkg_next_close;
    }

    return bpg_net_pkg_next_go_without_connection_id;
}

net_ep_t bpg_net_agent_process_reply(bpg_net_agent_t agent, uint64_t client_id, uint32_t connection_id) {
    net_ep_t ep;

    ep = NULL;

    if (connection_id != BPG_INVALID_CONNECTION_ID) {
        ep = net_ep_find(gd_app_net_mgr(agent->m_app), connection_id);
        if (ep == NULL) {
            CPE_ERROR(
                agent->m_em, "%s: net_agent_process_reply: connection of id %d not exist!",
                bpg_net_agent_name(agent), (int)connection_id);
            return NULL;
        }

        if (client_id != 0) {
            struct bpg_net_agent_binding * binding =
                bpg_net_agent_binding_find_by_connection_id(agent, connection_id);
            if (binding == NULL) {
                if (agent->m_debug) {
                    CPE_INFO(
                        agent->m_em, "%s: ep %d: binding: client %d no binding, accept replay!",
                        bpg_net_agent_name(agent), (int)connection_id, (int)client_id);
                }

                if (bpg_net_agent_binding_create(agent, client_id, connection_id) != 0) {
                    CPE_ERROR(
                        agent->m_em, "%s: ep %d: binding: create binding fail!",
                        bpg_net_agent_name(agent), (int)connection_id);
                }
            }
            else if (binding->m_client_id != client_id) {
                uint64_t old_client_id = binding->m_client_id;
                bpg_net_agent_binding_free(agent, binding);

                if (bpg_net_agent_binding_create(agent, client_id, connection_id) != 0) {
                    CPE_ERROR(
                        agent->m_em, "%s: ep %d: binding: create binding fail!",
                        bpg_net_agent_name(agent), (int)connection_id);
                }
                else {
                    if (agent->m_debug) {
                        CPE_INFO(
                            agent->m_em, "%s: ep %d: binding: client %d replace old binding %d!",
                            bpg_net_agent_name(agent), (int)connection_id, (int)client_id, (int)old_client_id);
                    }
                }
            }
        }            

        return ep;
    }

    if (client_id != 0) {
        struct bpg_net_agent_binding * binding =
            bpg_net_agent_binding_find_by_client_id(agent, client_id);
        if (binding == NULL) {
            CPE_ERROR(
                agent->m_em, "%s: net_agent_process_reply: connection binding of client %d not exist!",
                bpg_net_agent_name(agent), (int)client_id);
            return NULL;
        }

        ep = net_ep_find(gd_app_net_mgr(agent->m_app), binding->m_connection_id);
        if (ep == NULL) {
            CPE_ERROR(
                agent->m_em, "%s: net_agent_process_reply: connection id %d of client %d not exist!",
                bpg_net_agent_name(agent), (int)binding->m_connection_id, (int)client_id);
            return NULL;
        }
    }

    return ep;
}

int bpg_net_agent_binding_create(
    bpg_net_agent_t mgr,
    uint64_t client_id,
    uint32_t connection_id)
{
    struct bpg_net_agent_binding * binding;

    binding = mem_alloc(mgr->m_alloc, sizeof(struct bpg_net_agent_binding));
    if (binding == NULL) return -1;

    binding->m_client_id = client_id;
    binding->m_connection_id = connection_id;

    cpe_hash_entry_init(&binding->m_hh_client);
    cpe_hash_entry_init(&binding->m_hh_connection);

    if (cpe_hash_table_insert_unique(&mgr->m_cliensts, binding) != 0) {
        CPE_ERROR(
            mgr->m_em, "%s: create client binding %d ==> %d: client is already exist!",
            bpg_net_agent_name(mgr), (int)client_id, (int)connection_id);
        mem_free(mgr->m_alloc, binding);
        return -1;
    }

    if (cpe_hash_table_insert_unique(&mgr->m_connections, binding) != 0) {
        CPE_ERROR(
            mgr->m_em, "%s: create client binding %d ==> %d: connection is already exist!",
            bpg_net_agent_name(mgr), (int)client_id, (int)connection_id);
        cpe_hash_table_remove_by_ins(&mgr->m_cliensts, binding);
        mem_free(mgr->m_alloc, binding);
        return -1;
    }

    if (mgr->m_debug) {
        CPE_INFO(
            mgr->m_em, "%s: create client binding %d ==> %d: success!",
            bpg_net_agent_name(mgr), (int)client_id, (int)connection_id);
    }

    return 0;
}

void bpg_net_agent_binding_free(bpg_net_agent_t mgr, struct bpg_net_agent_binding * binding) {
    cpe_hash_table_remove_by_ins(&mgr->m_cliensts, binding);
    cpe_hash_table_remove_by_ins(&mgr->m_connections, binding);
    mem_free(mgr->m_alloc, binding);
}

struct bpg_net_agent_binding *
bpg_net_agent_binding_find_by_client_id(bpg_net_agent_t mgr, uint64_t client_id) {
    struct bpg_net_agent_binding key;
    key.m_client_id = client_id;

    return (struct bpg_net_agent_binding *)cpe_hash_table_find(&mgr->m_cliensts, &key);
}

struct bpg_net_agent_binding *
bpg_net_agent_binding_find_by_connection_id(bpg_net_agent_t mgr, uint32_t connection_id) {
    struct bpg_net_agent_binding key;
    key.m_connection_id = connection_id;

    return (struct bpg_net_agent_binding *)cpe_hash_table_find(&mgr->m_connections, &key);
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


