#ifndef GD_DP_NODE_H
#define GD_DP_NODE_H
#include "cpe/utils/hash_string.h"
#include "gd/nm/nm_types.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_node_t
gd_dp_node_create(
    gd_nm_mgr_t nmm,
    const char * name,
    const char * replayRsp,
    size_t capacity);

void gd_dp_node_free(gd_dp_node_t node);
const char * gd_dp_node_name(gd_dp_node_t node);
cpe_hash_string_t gd_dp_node_name_hs(gd_dp_node_t node);

const char * gd_dp_node_replay(gd_dp_node_t node);
cpe_hash_string_t gd_dp_node_replay_hs(gd_dp_node_t node);

void * gd_dp_node_data(gd_dp_node_t node);
size_t gd_dp_node_capacity(gd_dp_node_t node);

#ifdef __cplusplus
}
#endif

#endif
