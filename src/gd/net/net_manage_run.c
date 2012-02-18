#include <assert.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_manage.h"
#include "net_internal_ops.h"

int gd_net_mgr_tick(gd_net_mgr_t nmgr) {
    return 0;
}

int gd_net_mgr_run(gd_net_mgr_t nmgr) {
    ev_run(nmgr->m_ev_loop, 0);
    return 0;
}
