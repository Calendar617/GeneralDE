#ifndef GD_NM_MANAGE_H
#define GD_NM_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_nm_mgr_t gd_nm_mgr_create(mem_allocrator_t alloc);
void gd_nm_mgr_free(gd_nm_mgr_t nmm);

gd_nm_node_t gd_nm_group_create(gd_nm_mgr_t nmm, const char * name, size_t capacity);
int gd_nm_group_add_member(gd_nm_node_t grp, gd_nm_node_t sub);

gd_nm_node_t gd_nm_instance_create(gd_nm_mgr_t nmm, const char * name, size_t capacity);

void gd_nm_node_free(gd_nm_node_t node);

void gd_nm_node_set_type(gd_nm_node_t node, gd_nm_node_type_t type);

void gd_nm_group_free_members(gd_nm_node_t node);

#ifdef __cplusplus
}
#endif

#endif
