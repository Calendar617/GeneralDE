#include <assert.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_manage.h"
#include "net_internal_ops.h"

static void gd_net_mgr_process_svrs_init(gd_net_mgr_t nmgr) {
    gd_net_svr_list_t still_init_svrs;
    gd_net_svr_t svr;
    gd_net_svr_state_t nextState;

    TAILQ_INIT(&still_init_svrs);

    while(!TAILQ_EMPTY(&nmgr->m_svrs_init)) {
        svr = TAILQ_FIRST(&nmgr->m_svrs_init);

        nextState = g_net_svr_ops[svr->m_type].m_init(svr);
        switch(nextState) {
        case gd_net_svr_state_init: {
            TAILQ_REMOVE(&nmgr->m_svrs_init, svr, m_svr_next);
            TAILQ_INSERT_TAIL(&still_init_svrs, svr, m_svr_next);
            break;
        }
        default:
            gd_net_svr_update_state(svr, nextState);
            break;
        }
    }

    while(!TAILQ_EMPTY(&still_init_svrs)) {
        svr = TAILQ_FIRST(&still_init_svrs);
        TAILQ_REMOVE(&still_init_svrs, svr, m_svr_next);
        TAILQ_INSERT_TAIL(&nmgr->m_svrs_init, svr, m_svr_next);
    }
}

int gd_net_mgr_tick(gd_net_mgr_t nmgr) {
    do {
        gd_net_mgr_process_svrs_init(nmgr);
        ev_loop(nmgr->m_ev_loop, EVLOOP_NONBLOCK);
    } while(!TAILQ_EMPTY(&nmgr->m_svrs_init));

    return 0;
}
