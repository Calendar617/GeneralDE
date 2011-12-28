#include <assert.h>
#include "om_oid_alloc.h"

int gd_om_oid_alloc_init(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc) {
    assert(oid_alloc);

    oid_alloc->m_range_capacity = 0;
    oid_alloc->m_range_count = 0;
    oid_alloc->m_ranges = NULL;

    return 0;
}

void gd_om_oid_alloc_fini(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc) {
    assert(oid_alloc);

    if (oid_alloc->m_ranges) {
        mem_free(alloc, oid_alloc->m_ranges);
        oid_alloc->m_ranges = NULL;
    }

    oid_alloc->m_range_count = 0;
    oid_alloc->m_range_capacity = 0;
}

int32_t gd_om_oid_alloc(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc) {
    assert(oid_alloc);

    return -1;
}

void gd_om_oid_free(struct gd_om_oid_allocrator * oid_alloc, int32_t oid, mem_allocrator_t alloc) {
    assert(oid_alloc);
}

void gd_om_oid_add_range(struct gd_om_oid_allocrator * oid_alloc, int32_t oid_start, int32_t oid_count, mem_allocrator_t alloc) {
    assert(oid_alloc);
}

