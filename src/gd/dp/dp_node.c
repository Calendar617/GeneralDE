#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/dp/dp_node.h"
#include "dp_internal_types.h"

gd_dp_node_t
gd_dp_node_create(
    gd_nm_mgr_t nmm,
    const char * name,
    const char * replay,
    size_t capacity)
{
    size_t replayLen;
    gd_dp_node_t r;
    gd_nm_node_t nmNode;
    char * buf;

    replayLen = cpe_hs_len_to_binary_len(strlen(replay));

    nmNode = gd_nm_instance_create(
        nmm, name,
        replayLen + sizeof(struct gd_dp_node) + capacity);
    if (nmNode == NULL) return NULL;

    buf = gd_nm_node_data(nmNode);
    cpe_hs_init(buf, replayLen, replay);

    r = (gd_dp_node_t)(buf + replayLen);
    r->m_nm_node = nmNode;
    r->m_replay = (cpe_hash_string_t)buf;

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
