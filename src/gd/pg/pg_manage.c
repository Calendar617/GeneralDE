#include <assert.h>
#include "gd/pg/pg_manage.h"
#include "pg_internal_ops.h"

gd_pg_mgr_t
gd_pg_mgr_create(
    mem_allocrator_t alloc,
    gd_pg_backend_t backend,
    size_t pageSize)
{
    gd_pg_mgr_t pgm = mem_alloc(alloc, sizeof(struct gd_pg_mgr));
    if (pgm == NULL) return NULL;

    pgm->m_alloc = alloc;
    pgm->m_backend = backend;
    pgm->m_pageSize = pageSize;

    return pgm;
}

void gd_pg_mgr_free(gd_pg_mgr_t mgr) {
    
}
