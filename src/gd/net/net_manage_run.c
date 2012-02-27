#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_manage.h"
#include "net_internal_ops.h"

void gd_net_mgr_break(gd_net_mgr_t nmgr) {
    ev_break(nmgr->m_ev_loop, EVBREAK_ONE);
}

void gd_net_mgr_stop(gd_net_mgr_t nmgr) {
    ev_break(nmgr->m_ev_loop, EVBREAK_ALL);
}

int gd_net_mgr_tick(gd_net_mgr_t nmgr) {
    ev_run(nmgr->m_ev_loop, EVRUN_NOWAIT);
    return 0;
}

int gd_net_mgr_run(gd_net_mgr_t nmgr) {
    ev_run(nmgr->m_ev_loop, 0);
    return 0;
}
