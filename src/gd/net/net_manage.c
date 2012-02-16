#include <assert.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "gd/net/net_manage.h"
#include "gd/net/net_chanel.h"
#include "gd/net/net_endpoint.h"
#include "net_internal_ops.h"

gd_net_mgr_t
gd_net_mgr_create(mem_allocrator_t alloc, error_monitor_t em) {
    gd_net_mgr_t nmgr;

    nmgr = (gd_net_mgr_t)mem_alloc(alloc, sizeof(struct gd_net_mgr));
    if (nmgr == NULL) return NULL;
    bzero(nmgr, sizeof(struct gd_net_mgr));

    nmgr->m_ev_loop = ev_loop_new(EVFLAG_AUTO);
    if (!nmgr->m_ev_loop) {
        CPE_ERROR(em, "create event loop fail!");
        mem_free(alloc, nmgr);
        return NULL;
    }

    nmgr->m_alloc = alloc;
    nmgr->m_em = em;

    TAILQ_INIT(&nmgr->m_chanels);
    TAILQ_INIT(&nmgr->m_endpoints);
    TAILQ_INIT(&nmgr->m_listeners);

    return nmgr;
}

void gd_net_mgr_free(gd_net_mgr_t nmgr) {
    gd_net_chanel_t chanel;
    gd_net_ep_t ep;
    assert(nmgr);

    ev_loop_destroy(nmgr->m_ev_loop);
    nmgr->m_ev_loop = NULL;

    while((ep = TAILQ_FIRST(&nmgr->m_endpoints))) {
        gd_net_ep_free(ep);
    }

    while((chanel = TAILQ_FIRST(&nmgr->m_chanels))) {
        gd_net_chanel_free(chanel);
    }

    mem_free(nmgr->m_alloc, nmgr);
}

error_monitor_t gd_net_mgr_em(gd_net_mgr_t nmgr) {
    return nmgr->m_em;
}
