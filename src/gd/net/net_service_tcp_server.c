#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_unistd.h"
#include "net_internal_ops.h"

gd_net_svr_t
gd_net_svr_create_tcp_server(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    const char * ip,
    short port)
{
    gd_net_svr_t svr;
    struct gd_net_svr_tcp_server * svr_tcp;
    int flags;

    svr = gd_net_svr_crate_i(
        nmgr,
        gd_net_svr_type_tcp_server,
        name, readChanel, writeChanel,
        sizeof(struct gd_net_svr_tcp_server));
    if (svr == NULL) return NULL;

    svr_tcp = (struct gd_net_svr_tcp_server *)svr;

    strncpy(
        svr_tcp->m_ip,
        ip,
        sizeof(svr_tcp->m_ip));
    svr_tcp->m_port = port;

    svr_tcp->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_tcp->m_fd == -1) {
        CPE_ERROR(nmgr->m_em, "create tcp server: socke fail, errno=%d!", errno);
        gd_net_svr_free_i(svr);
        return NULL;
    }

    flags = fcntl(svr_tcp->m_fd, F_GETFL, 0);
    fcntl(svr_tcp->m_fd, F_SETFL, flags | O_NONBLOCK);

    if (gd_net_svr_fd_add(nmgr, svr, svr_tcp->m_fd) != 0) {
        close(svr_tcp->m_fd);
        gd_net_svr_free_i(svr);
        return NULL;
    }

    return svr;
}

void gd_net_svr_free_tcp_server(gd_net_svr_t svr) {
    struct gd_net_svr_tcp_server * svr_tcp;

    assert(svr->m_type == gd_net_svr_type_tcp_server);

    svr_tcp = (struct gd_net_svr_tcp_server *)svr;

    gd_net_svr_fd_remove(svr->m_mgr, svr_tcp->m_fd);

    close(svr_tcp->m_fd);

    gd_net_svr_free_i(svr);
}

