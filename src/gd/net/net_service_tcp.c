#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_service_tcp.h"
#include "net_internal_ops.h"

gd_net_svr_t
gd_net_svr_create_tcp(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    const char * ip,
    short port)
{
    gd_net_svr_t svr;
    struct gd_net_svr_tcp * svr_tcp;

    svr = gd_net_svr_crate_i(
        nmgr,
        gd_net_svr_type_tcp,
        name, readChanel, writeChanel,
        sizeof(struct gd_net_svr_tcp));
    if (svr == NULL) return NULL;

    svr_tcp = (struct gd_net_svr_tcp *)svr;

    strncpy(
        svr_tcp->m_ip,
        ip,
        sizeof(svr_tcp->m_ip));
    svr_tcp->m_port = port;
    return svr;
}

const char *
gd_net_svr_tcp_ip(gd_net_svr_t svr) {
    assert(svr);
    assert(svr->m_type = gd_net_svr_type_tcp);

    return ((struct gd_net_svr_tcp *)svr)->m_ip;
}

short gd_net_svr_tcp_port(gd_net_svr_t svr) {
    assert(svr);
    assert(svr->m_type = gd_net_svr_type_tcp);

    return ((struct gd_net_svr_tcp *)svr)->m_port;
}
