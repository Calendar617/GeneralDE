#include <assert.h>
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "nm_internal_ops.h"

gd_nm_node_t
gd_nm_instance_create(gd_nm_mgr_t nmm, cpe_hash_string_t name, size_t capacity) {
    struct gd_nm_instance * instance;

    assert(nmm);
    assert(name);

    instance = (struct gd_nm_instance *)
        gd_nm_node_alloc(
            nmm, name,
            gd_nm_node_instance, sizeof(struct gd_nm_instance),
            capacity);
    if (instance == NULL) return NULL;

    if (cpe_hash_table_insert_unique(&nmm->m_nodes, instance) != 0) {
        gd_nm_instance_free_from_mgr(instance);
        return NULL;
    }

    return (gd_nm_node_t)instance;
}
