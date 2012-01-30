#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_unistd.h"
#include "ev.h"
#include "cpe/pal/pal_strings.h"
#include "gd/net/net_manage.h"
#include "gd/net/net_chanel.h"
#include "gd/net/net_service.h"
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
    nmgr->m_fds = NULL;
    nmgr->m_fds_capacity = 0;

    nmgr->m_control_fd_listen = -1;
    nmgr->m_control_fd_client = -1;
    nmgr->m_control_fd_svr = -1;

    TAILQ_INIT(&nmgr->m_chanels);
    TAILQ_INIT(&nmgr->m_svrs_init);
    TAILQ_INIT(&nmgr->m_svrs_runing);
    TAILQ_INIT(&nmgr->m_svrs_shutingdown);

    if (gd_net_mgr_create_controler(nmgr) != 0) {
        ev_loop_destroy(nmgr->m_ev_loop);
        nmgr->m_ev_loop = NULL;
        mem_free(alloc, nmgr);
        return NULL;
    }

    return nmgr;
}

void gd_net_mgr_free(gd_net_mgr_t nmgr) {
    gd_net_chanel_t chanel;
    gd_net_svr_t svr;
    assert(nmgr);

    gd_net_mgr_free_controler(nmgr);

    ev_loop_destroy(nmgr->m_ev_loop);
    nmgr->m_ev_loop = NULL;

    if (nmgr->m_fds) {
        mem_free(nmgr->m_alloc, nmgr->m_fds);
        nmgr->m_fds = NULL;
        nmgr->m_fds_capacity = 0;
    }

    while((svr = TAILQ_FIRST(&nmgr->m_svrs_init))) {
        gd_net_svr_free(svr);
    }

    while((chanel = TAILQ_FIRST(&nmgr->m_chanels))) {
        gd_net_chanel_free(chanel);
    }

    mem_free(nmgr->m_alloc, nmgr);
}

int gd_net_svr_fd_add(
    gd_net_mgr_t nmgr,
    gd_net_svr_t svr,
    int fd)
{
    if (nmgr->m_fds == NULL || fd >= nmgr->m_fds_capacity) {
        size_t new_capacity;
        gd_net_svr_t * new_fds;

        new_capacity =
            nmgr->m_fds_capacity < 256
            ? 256
            : nmgr->m_fds_capacity * 2;
        
        new_fds =
            mem_alloc(
                nmgr->m_alloc,
                sizeof(gd_net_svr_t *) * new_capacity);
        bzero(new_fds, sizeof(gd_net_svr_t *) * new_capacity);

        if (new_fds == NULL) {
            CPE_ERROR(nmgr->m_em, "add fd: alloc new buf fail!");
            return -1;
        }

        memcpy(
            new_fds,
            nmgr->m_fds,
            sizeof(gd_net_svr_t*) * nmgr->m_fds_capacity);
        mem_free(nmgr->m_alloc, nmgr->m_fds);

        nmgr->m_fds = new_fds;
        nmgr->m_fds_capacity = new_capacity;
    }

    if (nmgr->m_fds[fd]) {
        CPE_ERROR(nmgr->m_em, "add fd: fd(%d) already alloc!", fd);
        return -1;
    }

    nmgr->m_fds[fd] = svr;
    return 0;
}

void gd_net_svr_fd_remove(
    gd_net_mgr_t nmgr,
    int fd)
{
    assert(nmgr->m_fds);
    assert(nmgr->m_fds_capacity > fd);

    nmgr->m_fds[fd] = NULL;
}

error_monitor_t gd_net_mgr_em(gd_net_mgr_t nmgr) {
    return nmgr->m_em;
}

int gd_net_mgr_run(gd_net_mgr_t nmgr) {
    ev_loop(nmgr->m_ev_loop, 0);
    return 0;
}
