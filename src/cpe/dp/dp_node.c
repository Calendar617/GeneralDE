#include <assert.h>
#include <string.h>
#include "cpe/nm/nm_manage.h"
#include "cpe/nm/nm_read.h"
#include "cpe/dp/dp_node.h"
#include "dp_internal_types.h"

static void dp_node_destory(nm_node_t node) {
    dp_node_t r = (dp_node_t)nm_node_data(node);

    if (r->m_type && r->m_type->destruct) {
        r->m_type->destruct(r);
    }
}

static struct nm_node_type g_dp_node_type = {
    "dp_node"
    , dp_node_destory
};

dp_node_t
dp_node_create(
    nm_mgr_t nmm,
    const char * name,
    dp_node_type_t type,
    size_t capacity)
{
    dp_node_t r;
    nm_node_t nmNode;

    assert(nmm);
    assert(name);
    assert(type);

    nmNode = nm_instance_create(
        nmm, name,
        sizeof(struct dp_node) + capacity);
    if (nmNode == NULL) return NULL;

    nm_node_set_type(nmNode, &g_dp_node_type);

    r = (dp_node_t)nm_node_data(nmNode);
    r->m_nm_node = nmNode;
    r->m_type = type;
    return r;
}

void dp_node_free(dp_node_t node) {
    if (node == NULL) return;
    nm_node_free(node->m_nm_node);
}

void * dp_node_data(dp_node_t node) {
    return node + 1;
}

size_t dp_node_capacity(dp_node_t node) {
    return nm_node_capacity(node->m_nm_node) - sizeof(struct dp_node);
}

const char * dp_node_name(dp_node_t node) {
    return nm_node_name(node->m_nm_node);
}

cpe_hash_string_t dp_node_name_hs(dp_node_t node) {
    return nm_node_name_hs(node->m_nm_node);
}

dp_node_type_t dp_node_type(dp_node_t node) {
    return node->m_type;
}
