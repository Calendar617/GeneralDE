#ifndef GD_OM_PAGE_RANGE_H
#define GD_OM_PAGE_RANGE_H
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_oid_range {
    int32_t m_start;
    int32_t m_count;
};

struct gd_om_oid_allocrator {
    size_t m_range_capacity;
    size_t m_range_count;
    struct gd_om_pagne_range * m_ranges;
};

int gd_om_oid_alloc_init(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc);
void gd_om_oid_alloc_fini(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc);

int32_t gd_om_oid_alloc(struct gd_om_oid_allocrator * oid_alloc, mem_allocrator_t alloc);
void gd_om_oid_free(struct gd_om_oid_allocrator * oid_alloc, int32_t oid, mem_allocrator_t alloc);
void gd_om_oid_add_range(struct gd_om_oid_allocrator * oid_alloc, int32_t oid_start, int32_t oid_count, mem_allocrator_t alloc);

#ifdef __cplusplus
}
#endif

#endif
