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

void gd_nm_node_free_from_mgr_base(gd_nm_node_t node);
void gd_nm_node_free_from_mgr(gd_nm_node_t node);

uint32_t gd_nm_node_hash(const gd_nm_node_t node);
int gd_nm_node_cmp(const gd_nm_node_t l, const gd_nm_node_t r);

/*group operations*/
void gd_nm_group_free_from_mgr(struct gd_nm_group * group);

/*instance operations*/
#define gd_nm_instance_free_from_mgr(instancee) gd_nm_node_free_from_mgr_base((gd_nm_node_t)instancee)

/*binding operations*/
struct gd_nm_binding * gd_nm_binding_get(gd_nm_mgr_t nmm);
void gd_nm_binding_put(gd_nm_mgr_t nmm, struct gd_nm_binding * binding);

void gd_nm_binding_free_from_group(struct gd_nm_binding * binding);
void gd_nm_binding_free_from_node(struct gd_nm_binding * binding);
struct gd_nm_binding *
gd_nm_binding_create(struct gd_nm_group * group, gd_nm_node_t node);

uint32_t gd_nm_binding_node_name_hash(const struct gd_nm_binding * binding);
int gd_nm_binding_node_name_cmp(const struct gd_nm_binding * l, const struct gd_nm_binding * r);

#ifdef __cplusplus
}
#endif

#endif
