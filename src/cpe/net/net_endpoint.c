#include <assert.h>
#include "cpe/pal/pal_socket.h"
#include "cpe/pal/pal_unistd.h"
#include "cpe/pal/pal_string.h"
#include "cpe/net/net_endpoint.h"
#include "cpe/net/net_chanel.h"
#include "cpe/net/net_connector.h"
#include "net_internal_ops.h"

static void net_ep_cb(EV_P_ ev_io *w, int revents);

net_ep_t
net_ep_create(net_mgr_t nmgr) {
    net_ep_t ep;

    assert(nmgr);

    ep = net_ep_pages_alloc_ep(nmgr);
    if (ep == NULL) return NULL;

    assert(ep->m_id != GD_NET_EP_INVALID_ID);

    ep->m_mgr = nmgr;
    ep->m_chanel_r = NULL;
    ep->m_chanel_w = NULL;
    ep->m_connector = NULL;
    ep->m_fd = -1;
    ep->m_process_fun = NULL;
    ep->m_process_ctx = NULL;

    ep->m_watcher.data = ep;
    ev_init(&ep->m_watcher, net_ep_cb);

    return ep;
}

void net_ep_free(net_ep_t ep) {
    if (ep->m_connector) {
        net_connector_unbind(ep->m_connector);
    }

    if (net_ep_is_open(ep)) {
        net_ep_close_i(ep, net_ep_event_close_by_shutdown);
    }

    if (ep->m_chanel_w) {
        net_chanel_free(ep->m_chanel_w);
        ep->m_chanel_w = NULL;
    }

    if (ep->m_chanel_r) {
        net_chanel_free(ep->m_chanel_r);
        ep->m_chanel_r = NULL;
    }

    net_ep_pages_free_ep(ep);
}

net_ep_id_t net_ep_id(net_ep_t ep) {
    return ep->m_id;
}

void net_ep_set_processor(net_ep_t ep, net_process_fun_t process_fun, void * process_ctx) {
    ep->m_process_fun = process_fun;
    ep->m_process_ctx = process_ctx;

    if (process_fun && net_ep_is_open(ep)) {
        process_fun(ep, process_ctx, net_ep_event_open);
    }
}

#define net_ep_calc_ev_events(ep)                               \
    (((ep->m_chanel_r                                           \
       && ep->m_chanel_r->m_state != net_chanel_state_full)     \
      ? EV_READ : 0)                                            \
     |                                                          \
     ((ep->m_chanel_w                                           \
       && ep->m_chanel_w->m_state != net_chanel_state_empty)    \
      ? EV_WRITE : 0))

#define net_ep_update_events(ep, old_events)                        \
    if (ep->m_fd >= 0) {                                            \
        int new_events = net_ep_calc_ev_events(ep);                 \
        if (old_events != new_events) {                             \
            if (old_events)                                         \
                ev_io_stop(ep->m_mgr->m_ev_loop, &ep->m_watcher);   \
            if (new_events) {                                       \
                ev_io_set(&ep->m_watcher, ep->m_fd, new_events);    \
                ev_io_start(ep->m_mgr->m_ev_loop, &ep->m_watcher);  \
            }                                                       \
        }                                                           \
    } while(0)

net_chanel_t
net_ep_chanel_r(net_ep_t ep) {
    return ep->m_chanel_r;
}

void net_ep_set_chanel_r(net_ep_t ep, net_chanel_t chanel) {
    int old_events;

    old_events = net_ep_calc_ev_events(ep);

    if (ep->m_chanel_r) {
        net_chanel_free(ep->m_chanel_r);
    }

    ep->m_chanel_r = chanel;

    net_ep_update_events(ep, old_events);
}

net_chanel_t
net_ep_chanel_w(net_ep_t ep) {
    return ep->m_chanel_w;
}

void net_ep_set_chanel_w(net_ep_t ep, net_chanel_t chanel) {
    int old_events;

    old_events = net_ep_calc_ev_events(ep);

    if (ep->m_chanel_w) {
        net_chanel_free(ep->m_chanel_w);
    }

    ep->m_chanel_w = chanel;

    net_ep_update_events(ep, old_events);
}

int net_ep_is_open(net_ep_t ep) {
    return ep->m_fd < 0 ? 0 : 1;
}

void net_ep_close_i(net_ep_t ep, net_ep_event_t ev) {
    assert(ep);
    assert(ev == net_ep_event_close_by_user
           || ev == net_ep_event_close_by_peer
           || ev == net_ep_event_close_by_error
           || ev == net_ep_event_close_by_shutdown);

    if (ep->m_fd < 0) return;

#ifdef _MSC_VER
    if (ep->m_type = net_ep_socket) {
        net_socket_close(&ep->m_fd, ep->m_mgr->m_em);
    }
    else {
        CPE_ERROR(
            ep->m_mgr->m_em, "net_ep_close: close fail, errno=%d (%s)",
            cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
    }
#else
    if (close(ep->m_fd) != 0) {
        CPE_ERROR(
            ep->m_mgr->m_em, "net_ep_close: close fail, errno=%d (%s)",
            cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
    }
#endif

    if (net_ep_calc_ev_events(ep)) {
        ev_io_stop(ep->m_mgr->m_ev_loop, &ep->m_watcher);
    }

    ep->m_fd = -1;

    if (ep->m_process_fun) ep->m_process_fun(ep, ep->m_process_ctx, ev);
}

void net_ep_close(net_ep_t ep) {
    net_ep_close_i(ep, net_ep_event_close_by_user);
}

int net_ep_set_fd(net_ep_t ep, int fd) {
    assert(ep);

    if (net_ep_is_open(ep)) {
        CPE_ERROR(
            ep->m_mgr->m_em, "net_mgr: ep %d: set fd to a opend ep",
            (int)ep->m_id);
        net_ep_close_i(ep, net_ep_event_close_by_error);
    }

    ep->m_fd = fd;

    if (net_ep_is_open(ep)) {
        int events = net_ep_calc_ev_events(ep);
        if (events) {
            ev_io_set(&ep->m_watcher, ep->m_fd, events);
            ev_io_start(ep->m_mgr->m_ev_loop, &ep->m_watcher);
        }

        if (ep->m_process_fun)
            ep->m_process_fun(ep, ep->m_process_fun, net_ep_event_open);
    }

    return 0;
}

int ge_net_ep_send(net_ep_t ep, const void * buf, size_t size) {
    int old_events;
    int r;

    assert(ep);
    if (buf == NULL || size <= 0) return -1;
    if (ep->m_chanel_w == NULL) return -1;

    old_events = net_ep_calc_ev_events(ep);

    r = ep->m_chanel_w->m_type->read_from_buf(ep->m_chanel_w, buf, size);

    net_ep_update_events(ep, old_events);

    return r;
}

ssize_t ge_net_ep_rece(net_ep_t ep, void * buf, size_t capacity) {
    int old_events;
    ssize_t r;

    assert(ep);
    if (buf == NULL) return -1;
    if (ep->m_chanel_r == NULL) return -1;

    old_events = net_ep_calc_ev_events(ep);

    r = ep->m_chanel_r->m_type->write_to_buf(ep->m_chanel_r, buf, capacity);

    net_ep_update_events(ep, old_events);

    return r;
}

void * ge_net_ep_peek(net_ep_t ep, void * buf, size_t size) {
    assert(ep);
    if (ep->m_chanel_r == NULL) return NULL;

    return ep->m_chanel_r->m_type->peek(ep->m_chanel_r, buf, size);
}

void ge_net_ep_erase(net_ep_t ep, size_t size) {
    int old_events;

    assert(ep);
    if (ep->m_chanel_r == NULL) return;

    old_events = net_ep_calc_ev_events(ep);

    ep->m_chanel_r->m_type->erase(ep->m_chanel_r, size);

    net_ep_update_events(ep, old_events);
}

void net_ep_cb(EV_P_ ev_io *w, int revents) {
    net_ep_t ep;
    int old_events;

    ep = (net_ep_t)w->data;
    assert(ep);

    old_events = net_ep_calc_ev_events(ep);

    if (revents & EV_READ) {
        if (ep->m_chanel_r->m_type->read_from_net(ep->m_chanel_r, ep->m_fd) != 0) {
            CPE_ERROR(
                ep->m_mgr->m_em,
                "net_mgr: ep %d: read data error, errno=%d (%s)",
                ep->m_id, cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
            net_ep_close_i(ep, net_ep_event_close_by_error);
            return;
        }
        else {
            if (ep->m_process_fun) {
                ep->m_process_fun(ep, ep->m_process_ctx, net_ep_event_read);
            }
        }
    }

    if (revents & EV_WRITE) {
        if (ep->m_chanel_w->m_type->write_to_net(ep->m_chanel_w, ep->m_fd) != 0) {
            CPE_ERROR(
                ep->m_mgr->m_em,
                "net_mgr: ep %d: write data error, errno=%d (%s)",
                ep->m_id, cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
            net_ep_close_i(ep, net_ep_event_close_by_error);
            return;
        }
    }

    net_ep_update_events(ep, old_events);
}
