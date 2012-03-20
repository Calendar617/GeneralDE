#include <assert.h>
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_instance_create(gd_nm_mgr_t nmm, const char *  name, size_t capacity) {
    assert(nmm);
    assert(name);

    return gd_nm_node_alloc(
        nmm, name,
        gd_nm_node_instance, sizeof(struct gd_nm_instance),
        capacity);
}
