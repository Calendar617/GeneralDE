#include <assert.h>
#include "gd/net/net_manage.h"
#include "gd/net/net_chanel.h"
#include "net_internal_types.h"

gd_net_mgr_t gd_net_mgr_create(mem_allocrator_t alloc) {
    gd_net_mgr_t nmgr = (gd_net_mgr_t)mem_alloc(alloc, sizeof(struct gd_net_mgr));
    if (nmgr == NULL) return NULL;

    nmgr->m_alloc = alloc;

    return nmgr;
}

void gd_net_mgr_free(gd_net_mgr_t nmgr) {
    gd_net_chanel_t chanel;
    assert(nmgr);

    while((chanel = TAILQ_FIRST(&nmgr->m_chanels))) {
        gd_net_chanel_free(chanel);
    }
}

void gd_net_mgr_run(gd_net_mgr_t nmgr) {
}
