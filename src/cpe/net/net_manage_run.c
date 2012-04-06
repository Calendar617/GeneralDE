#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/net/net_manage.h"
#include "net_internal_ops.h"

void net_mgr_break(net_mgr_t nmgr) {
    ev_break(nmgr->m_ev_loop, EVBREAK_ONE);
}

void net_mgr_stop(net_mgr_t nmgr) {
    ev_break(nmgr->m_ev_loop, EVBREAK_ALL);
}

int net_mgr_tick(net_mgr_t nmgr) {
    ev_run(nmgr->m_ev_loop, EVRUN_NOWAIT);
    return 0;
}

int net_mgr_run(net_mgr_t nmgr) {
    ev_run(nmgr->m_ev_loop, 0);
    return 0;
}
