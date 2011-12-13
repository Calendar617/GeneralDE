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

    buf = mem_alloc(
        nmm->m_alloc,
        cpe_hs_binary_len(name)
        + bodyLen
        + capacity);
    if (buf == NULL) return NULL;

    cpe_hs_copy((cpe_hash_string_t)buf, name);

    node = (gd_nm_node_t)
        (buf
         + cpe_hs_binary_len(name));

    node->m_mgr = nmm;
    node->m_type = type;
    node->m_name = (cpe_hash_string_t)buf;
    node->m_data_capacity = capacity;

    return node;
}

void gd_nm_node_free_i(gd_nm_node_t node) {
    if (node == NULL) return;
    mem_free(node->m_mgr->m_alloc, node->m_name);
}

