#ifndef GD_NM_MANAGE_H
#define GD_NM_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_nm_mgr_t gd_nm_mgr_create(mem_allocrator_t alloc, size_t maxGroupCount);
void gd_nm_mgr_free(gd_nm_mgr_t nmm);

gd_nm_node_t gd_nm_group_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity);
int gd_nm_group_add_member(gd_nm_node_t grp, gd_nm_node_t sub);

gd_nm_node_t gd_nm_instance_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif
