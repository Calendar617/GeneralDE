#ifndef GD_NM_INTERNAL_OPS_H
#define GD_NM_INTERNAL_OPS_H
#include "nm_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*mgr operations*/
#define gd_nm_mgr_group_mask(nmm) ((cpe_ba_t)(nmm + 1))

/*node operations*/
gd_nm_node_t
gd_nm_node_alloc(
    gd_nm_mgr_t nmm,
    cpe_hash_string_t name, gd_nm_node_type_t type,
    size_t bodyLen, size_t capacity);

void gd_nm_node_free_i(gd_nm_node_t node);

#ifdef __cplusplus
}
#endif

#endif
