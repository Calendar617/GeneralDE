#include <assert.h>
#include <errno.h>
#include "cpe/pal/pal_unistd.h"
#include "cpe/pal/pal_string.h"
#include "gd/net/net_endpoint.h"
#include "gd/net/net_chanel.h"
#include "gd/net/net_connector.h"
#include "net_internal_ops.h"

gd_net_ep_t
gd_net_ep_create(gd_net_mgr_t nmgr, gd_net_ep_type_t type) {
    gd_net_ep_t ep;

    assert(nmgr);

    ep = gd_net_ep_pages_alloc_ep(nmgr);
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

void gd_net_ep_free(gd_net_ep_t ep) {
    if (ep->m_connector) {
        gd_net_connector_unbind(ep->m_connector);
    }

    if (gd_net_ep_is_open(ep)) {
        gd_net_ep_close(ep);
    }

    if (ep->m_chanel_w) {
        gd_net_chanel_free(ep->m_chanel_w);
        ep->m_chanel_w = NULL;
    }

    if (ep->m_chanel_r) {
        gd_net_chanel_free(ep->m_chanel_r);
        ep->m_chanel_r = NULL;
    }

    gd_net_ep_pages_free_ep(ep);
}

gd_net_ep_type_t gd_net_ep_type(gd_net_ep_t ep) {
    return ep->m_type;
}

gd_net_ep_id_t gd_net_ep_id(gd_net_ep_t ep) {
    return ep->m_id;
}

gd_net_chanel_t
gd_net_ep_chanel_r(gd_net_ep_t ep) {
    return ep->m_chanel_r;
}

void gd_net_ep_set_chanel_r(gd_net_ep_t ep, gd_net_chanel_t chanel) {
    if (ep->m_chanel_r) {
        gd_net_chanel_free(ep->m_chanel_r);
    }

    ep->m_chanel_r = chanel;
}

gd_net_chanel_t
gd_net_ep_chanel_w(gd_net_ep_t ep) {
    return ep->m_chanel_w;
}

void gd_net_ep_set_chanel_w(gd_net_ep_t ep, gd_net_chanel_t chanel) {
    if (ep->m_chanel_w) {
        gd_net_chanel_free(ep->m_chanel_w);
    }

    ep->m_chanel_w = chanel;
}

int gd_net_ep_is_open(gd_net_ep_t ep) {
    return ep->m_fd < 0 ? 0 : 1;
}

void gd_net_ep_close(gd_net_ep_t ep) {
    if (ep->m_fd < 0) return;

#ifdef _MSC_VER
    if (ep->m_type = gd_net_ep_socket) {
        gd_net_socket_close(&ep->m_fd, ep->m_mgr->m_em);
    }
    else {
        CPE_ERROR(ep->m_mgr->m_em, "gd_net_ep_close: close fail, errno=%s", strerror(errno));
    }
#else
    if (close(ep->m_fd) != 0) {
        CPE_ERROR(ep->m_mgr->m_em, "gd_net_ep_close: close fail, errno=%s", strerror(errno));
    }
#endif

    ep->m_fd = -1;
}

int ge_net_ep_send(gd_net_ep_t ep, const void * buf, size_t size);
int ge_net_ep_rece(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_peek(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_erase(gd_net_ep_t ep, size_t size);

