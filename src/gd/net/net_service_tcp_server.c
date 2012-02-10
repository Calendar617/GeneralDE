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
    svr_tcp->m_fd = -1;

    return svr;
}

void gd_net_svr_free_tcp_server(gd_net_svr_t svr) {
    struct gd_net_svr_tcp_server * svr_tcp;

    assert(svr->m_type == gd_net_svr_type_tcp_server);

    svr_tcp = (struct gd_net_svr_tcp_server *)svr;
    assert(svr_tcp->m_fd == -1);

    gd_net_svr_free_i(svr);
}

gd_net_svr_state_t
gd_net_svr_init_tcp_server(gd_net_svr_t svr) {
    struct sockaddr_in addr;
    struct gd_net_svr_tcp_server * svr_tcp;
    error_monitor_t em;

    em = svr->m_mgr->m_em;
    svr_tcp = (struct gd_net_svr_tcp_server *)svr;

    bzero(&addr, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(svr_tcp->m_port);
    addr.sin_addr.s_addr = inet_addr(svr_tcp->m_ip);

    if (gd_net_socket_open(&svr_tcp->m_fd, em) != 0) {
        return gd_net_svr_state_init;
    }

    if (gd_net_socket_set_none_block(svr_tcp->m_fd, em) != 0) {
        gd_net_socket_close(&svr_tcp->m_fd, em);
        return gd_net_svr_state_init;
    }

    if(bind(svr_tcp->m_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        CPE_ERROR(em, "gd_net_svr_init_tcp_server: bind fail, error=%s!", strerror(errno));
        gd_net_socket_close(&svr_tcp->m_fd, em);
        return gd_net_svr_state_init;
    }

    if (listen(svr_tcp->m_fd, 1) != 0) {
        CPE_ERROR(em, "gd_net_svr_init_tcp_server: listen fail, error=%s!", strerror(errno));
        gd_net_socket_close(&svr_tcp->m_fd, em);
        return gd_net_svr_state_init;
    }

    if (gd_net_svr_fd_add(svr->m_mgr, svr, svr_tcp->m_fd) != 0) {
        gd_net_socket_close(&svr_tcp->m_fd, em);
        return gd_net_svr_state_init;
    }

    return gd_net_svr_state_runing;
}

void gd_net_svr_fini_tcp_server(gd_net_svr_t svr) {
    struct gd_net_svr_tcp_server * svr_tcp;
    svr_tcp = (struct gd_net_svr_tcp_server *)svr;

    gd_net_svr_fd_remove(svr->m_mgr, svr_tcp->m_fd);

    gd_net_socket_close(&svr_tcp->m_fd, svr_tcp->m_mgr->m_em);
}

const char *
gd_net_svr_tcp_server_ip(gd_net_svr_t svr) {
    assert(svr);
    assert(svr->m_type = gd_net_svr_type_tcp_server);

    return ((struct gd_net_svr_tcp_server *)svr)->m_ip;
}

short gd_net_svr_tcp_server_port(gd_net_svr_t svr) {
    assert(svr);
    assert(svr->m_type = gd_net_svr_type_tcp_server);

    return ((struct gd_net_svr_tcp_server *)svr)->m_port;
}
