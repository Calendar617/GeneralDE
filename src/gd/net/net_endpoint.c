#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_endpoint.h"
#include "net_internal_ops.h"

gd_net_ep_t
gd_net_ep_crate_i(
    gd_net_mgr_t nmgr,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    size_t total_capacity)
{
    gd_net_ep_t ep;
    char * buf;

    assert(nmgr);

    buf = (char *)mem_alloc(nmgr->m_alloc, total_capacity);
    if (buf == NULL) return NULL;

    ep = (gd_net_ep_t)buf;

    ep->m_mgr = nmgr;
    ep->m_chanel_read = readChanel;
    ep->m_chanel_write = writeChanel;

    TAILQ_INSERT_TAIL(&nmgr->m_endpoints, ep, m_ep_next);

    return ep;
}

void gd_net_ep_free_i(gd_net_ep_t ep) {
    TAILQ_REMOVE(&ep->m_mgr->m_endpoints, ep, m_ep_next);
    mem_free(ep->m_mgr->m_alloc, ep);
}

gd_net_chanel_t
gd_net_ep_chanel_read(gd_net_ep_t ep) {
    return ep->m_chanel_read;
}

gd_net_chanel_t
gd_net_ep_chanel_write(gd_net_ep_t ep) {
    return ep->m_chanel_write;
}

int ge_net_ep_send(gd_net_ep_t ep, const void * buf, size_t size);
int ge_net_ep_rece(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_peek(gd_net_ep_t ep, void * buf, size_t * size);
int ge_net_ep_erase(gd_net_ep_t ep, size_t size);

