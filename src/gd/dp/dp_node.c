#include <assert.h>
#include <string.h>
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/dp/dp_node.h"
#include "dp_internal_types.h"

static void gd_dp_node_destory(gd_nm_node_t node) {
    gd_dp_node_t r = (gd_dp_node_t)gd_nm_node_data(node);

    if (r->m_type && r->m_type->destruct) {
        r->m_type->destruct(r);
    }
}

static struct gd_nm_node_type g_dp_node_type = {
    "dp_node"
    , gd_dp_node_destory
};

gd_dp_node_t
gd_dp_node_create(
    gd_nm_mgr_t nmm,
    const char * name,
    gd_dp_node_type_t type,
    size_t capacity)
{
    gd_dp_node_t r;
    gd_nm_node_t nmNode;

    assert(nmm);
    assert(name);
    assert(type);

    nmNode = gd_nm_instance_create(
        nmm, name,
        sizeof(struct gd_dp_node) + capacity);
    if (nmNode == NULL) return NULL;

    gd_nm_node_set_type(nmNode, &g_dp_node_type);

    r = (gd_dp_node_t)gd_nm_node_data(nmNode);
    r->m_nm_node = nmNode;
    r->m_type = type;
    return r;
}

void gd_dp_node_free(gd_dp_node_t node) {
    if (node == NULL) return;
    gd_nm_node_free(node->m_nm_node);
}

void * gd_dp_node_data(gd_dp_node_t node) {
    return node + 1;
}

size_t gd_dp_node_capacity(gd_dp_node_t node) {
    return gd_nm_node_capacity(node->m_nm_node) - sizeof(struct gd_dp_node);
}

const char * gd_dp_node_name(gd_dp_node_t node) {
    return gd_nm_node_name(node->m_nm_node);
}

cpe_hash_string_t gd_dp_node_name_hs(gd_dp_node_t node) {
    return gd_nm_node_name_hs(node->m_nm_node);
}

gd_dp_node_type_t gd_dp_node_type(gd_dp_node_t node) {
    return node->m_type;
}
