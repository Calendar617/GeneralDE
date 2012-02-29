#include <assert.h>
#include "cpe/pal/pal_socket.h"
#include "cpe/pal/pal_unistd.h"
#include "cpe/pal/pal_string.h"
#include "cpe/net/net_endpoint.h"
#include "cpe/net/net_chanel.h"
#include "cpe/net/net_connector.h"
#include "net_internal_ops.h"

net_ep_t
net_ep_create(net_mgr_t nmgr, net_ep_type_t type) {
    net_ep_t ep;

    assert(nmgr);

    ep = net_ep_pages_alloc_ep(nmgr);
    if (ep == NULL) return NULL;

    assert(ep->m_id != GD_NET_EP_INVALID_ID);

    ep->m_mgr = nmgr;
    ep->m_type = type;
    ep->m_chanel_r = NULL;
    ep->m_chanel_w = NULL;
    ep->m_connector = NULL;
    ep->m_fd = -1;

    return ep;
}

void net_ep_free(net_ep_t ep) {
    if (ep->m_connector) {
        net_connector_unbind(ep->m_connector);
    }

    if (net_ep_is_open(ep)) {
        net_ep_close(ep);
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

net_ep_type_t net_ep_type(net_ep_t ep) {
    return ep->m_type;
}

net_ep_id_t net_ep_id(net_ep_t ep) {
    return ep->m_id;
}

net_chanel_t
net_ep_chanel_r(net_ep_t ep) {
    return ep->m_chanel_r;
}

void net_ep_set_chanel_r(net_ep_t ep, net_chanel_t chanel) {
    if (ep->m_chanel_r) {
        net_chanel_free(ep->m_chanel_r);
    }

    ep->m_chanel_r = chanel;
}

net_chanel_t
net_ep_chanel_w(net_ep_t ep) {
    return ep->m_chanel_w;
}

void net_ep_set_chanel_w(net_ep_t ep, net_chanel_t chanel) {
    if (ep->m_chanel_w) {
        net_chanel_free(ep->m_chanel_w);
    }

    ep->m_chanel_w = chanel;
}

int net_ep_is_open(net_ep_t ep) {
    return ep->m_fd < 0 ? 0 : 1;
}

void net_ep_close(net_ep_t ep) {
    if (ep->m_fd < 0) return;

#ifdef _MSC_VER
    if (ep->m_type = net_ep_socket) {
        net_socket_close(&ep->m_fd, ep->m_mgr->m_em);
    }
    else {
        CPE_ERROR(
            ep->m_mgr->m_em,
            "net_ep_close: close fail, errno=%d (%s)", cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
    }
#else
    if (close(ep->m_fd) != 0) {
        CPE_ERROR(
            ep->m_mgr->m_em,
            "net_ep_close: close fail, errno=%d (%s)", cpe_sock_errno(), cpe_sock_errstr(cpe_sock_errno()));
    }
#endif

    ep->m_fd = -1;
}

int ge_net_ep_send(net_ep_t ep, const void * buf, size_t size);
int ge_net_ep_rece(net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_peek(net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_erase(net_ep_t ep, size_t size);

