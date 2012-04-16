#ifndef CPE_DP_NODE_H
#define CPE_DP_NODE_H
#include "cpe/utils/hash_string.h"
#include "cpe/nm/nm_types.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dp_node_t
dp_node_create(
    nm_mgr_t nmm,
    const char * name,
    dp_node_type_t type,
    size_t capacity);

void dp_node_free(dp_node_t node);
const char * dp_node_name(dp_node_t node);
cpe_hash_string_t dp_node_name_hs(dp_node_t node);
dp_node_type_t dp_node_type(dp_node_t node);

void * dp_node_data(dp_node_t node);
size_t dp_node_capacity(dp_node_t node);

#ifdef __cplusplus
}
#endif

#endif
