#include "gd/nm/nm_manage.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_group_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity) {
    struct gd_nm_group * group;

    group = (struct gd_nm_group *)
        gd_nm_node_alloc(
            nmm, name,
            gd_nm_node_group, sizeof(struct gd_nm_group),
            capacity);
    if (group == NULL) return NULL;

    return (gd_nm_node_t)group;
}
