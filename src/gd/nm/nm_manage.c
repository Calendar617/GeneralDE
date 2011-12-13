#include "gd/nm/nm_manage.h"
#include "nm_internal_ops.h"

gd_nm_mgr_t gd_nm_mgr_create(mem_allocrator_t alloc, size_t maxGroupCount) {
    gd_nm_mgr_t nmm =
        (gd_nm_mgr_t)mem_alloc(
            alloc,
            sizeof(struct gd_nm_mgr));

    if (nmm == NULL) return NULL;

    nmm->m_alloc = alloc;
    nmm->m_maxGroupCount = maxGroupCount;

    return nmm;
}

void gd_nm_mgr_free(gd_nm_mgr_t nmm) {
    if (nmm == NULL) return;

    mem_free(nmm->m_alloc, nmm);
}
