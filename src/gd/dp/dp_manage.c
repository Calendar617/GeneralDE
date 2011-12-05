#include <strings.h>
#include "gd/dp/dp_manage.h"
#include "dp_internal_types.h"

gd_dp_manage_t gd_dp_manage_create(mem_allocrator_t alloc, gd_tl_t timerTl) {
    gd_dp_manage_t dpm;
    size_t allocSize = sizeof(struct gd_dp_manage);

    dpm = (gd_dp_manage_t)mem_alloc(alloc, allocSize);
    bzero(dpm, allocSize);

    dpm->m_alloc = alloc;
    dpm->m_timerTl = timerTl;

    return dpm;
}

void gd_dp_manage_free(gd_dp_manage_t dp) {
    if (dp == NULL) return;

    mem_free(dp->m_alloc, dp);
}
