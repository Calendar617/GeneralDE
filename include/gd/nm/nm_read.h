#ifndef GD_NM_NODE_H
#define GD_NM_NODE_H
#include "cpe/utils/memory.h"
#include "nm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*mgr operations*/
gd_nm_node_t gd_nm_mgr_find_node(gd_nm_mgr_t nmm, cpe_hash_string_t name);
int gd_nm_mgr_nodes(gd_nm_node_it_t it, gd_nm_mgr_t nmm);

/*node operations*/
const char * gd_nm_node_name(gd_nm_node_t node);
cpe_hash_string_t gd_nm_node_name_hs(gd_nm_node_t node);
gd_nm_node_category_t gd_nm_node_category(gd_nm_node_t node);
size_t gd_nm_node_capacity(gd_nm_node_t node);
void * gd_nm_node_data(gd_nm_node_t node);

int gd_nm_node_groups(gd_nm_node_it_t it, gd_nm_node_t node);

/*gruop operations*/
int gd_nm_group_members(gd_nm_node_it_t it, gd_nm_node_t group);

/*iterator operations*/
#define gd_nm_node_next(it) (it)->m_next_fun((it))

#ifdef __cplusplus
}
#endif

#endif
