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

    if (cpe_range_mgr_init(&nmgr->m_ep_ids, alloc) != 0) {
        CPE_ERROR(em, "init ep ids fail!");
        mem_free(alloc, nmgr);
        return NULL;
    }

    nmgr->m_ev_loop = ev_loop_new(EVFLAG_AUTO);
    if (!nmgr->m_ev_loop) {
        CPE_ERROR(em, "gd_net_mgr_create: create event loop fail!");
        cpe_range_mgr_fini(&nmgr->m_ep_ids);
        mem_free(alloc, nmgr);
        return NULL;
    }

    nmgr->m_alloc = alloc;
    nmgr->m_em = em;
    nmgr->m_ep_page_capacity = 0;
    nmgr->m_ep_pages = NULL;

    TAILQ_INIT(&nmgr->m_chanels);
    TAILQ_INIT(&nmgr->m_listeners);

    if (cpe_hash_table_init(
            &nmgr->m_connectors,
            alloc,
            (cpe_hash_fun_t)gd_net_connector_hash,
            (cpe_hash_cmp_t)gd_net_connector_cmp,
            CPE_HASH_OBJ2ENTRY(gd_net_connector, m_hh),
            256) != 0)
    {
        CPE_ERROR(em, "gd_net_mgr_create: init connector hash list fail!");
        cpe_range_mgr_fini(&nmgr->m_ep_ids);
        mem_free(alloc, nmgr);
        return NULL;
    }

    return nmgr;
}

void gd_net_mgr_free(gd_net_mgr_t nmgr) {
    gd_net_chanel_t chanel;

    assert(nmgr);

    /*free connectors*/
    gd_net_connectors_free(nmgr);
    cpe_hash_table_fini(&nmgr->m_connectors);

    /*free eps*/
    gd_net_ep_pages_free(nmgr);
    cpe_range_mgr_fini(&nmgr->m_ep_ids);

    /*free event loop*/
    ev_loop_destroy(nmgr->m_ev_loop);
    nmgr->m_ev_loop = NULL;

    /*TODO*/
    while((chanel = TAILQ_FIRST(&nmgr->m_chanels))) {
        gd_net_chanel_free(chanel);
    }

    mem_free(nmgr->m_alloc, nmgr);
}

error_monitor_t gd_net_mgr_em(gd_net_mgr_t nmgr) {
    return nmgr->m_em;
}
