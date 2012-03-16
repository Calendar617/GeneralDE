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
    const char * name, gd_nm_node_category_t category,
    size_t bodyLen, size_t capacity);

uint32_t gd_nm_node_hash(const gd_nm_node_t node);
int gd_nm_node_cmp(const gd_nm_node_t l, const gd_nm_node_t r);

/*binding operations*/
struct gd_nm_binding * gd_nm_binding_get(gd_nm_mgr_t nmm);
void gd_nm_binding_put(gd_nm_mgr_t nmm, struct gd_nm_binding * binding);

struct gd_nm_binding *
gd_nm_binding_create(struct gd_nm_group * group, gd_nm_node_t node);
void gd_nm_binding_free(struct gd_nm_binding * binding);

uint32_t gd_nm_binding_node_name_hash(const struct gd_nm_binding * binding);
int gd_nm_binding_node_name_cmp(const struct gd_nm_binding * l, const struct gd_nm_binding * r);

#ifdef __cplusplus
}
#endif

#endif
