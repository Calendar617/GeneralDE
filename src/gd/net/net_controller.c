#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_unistd.h"
#include "net_internal_ops.h"

int gd_net_mgr_create_controler_listener(gd_net_mgr_t nmgr) {
    struct sockaddr_in addr;

    bzero(&addr, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    nmgr->m_control_fd_listen = socket(AF_INET, SOCK_STREAM, 0);
    if (nmgr->m_control_fd_listen == -1) {
        CPE_ERROR(nmgr->m_em, "gd_net_mgr open listen socket fail, errno=%s", strerror(errno));
        return -1;
    }

    if(bind(nmgr->m_control_fd_listen, (struct sockaddr *)&addr, sizeof(addr)) == -1 ) {
        CPE_ERROR(nmgr->m_em, "gd_net_mgr bind listen socket fail, errno=%s", strerror(errno));
        close(nmgr->m_control_fd_listen);
        nmgr->m_control_fd_listen = -1;
        return -1;
    }

    if (listen(nmgr->m_control_fd_listen, 2) == -1) {
        CPE_ERROR(nmgr->m_em, "gd_net_mgr listen error, errno=%s", strerror(errno));
        close(nmgr->m_control_fd_listen);
        nmgr->m_control_fd_listen = -1;
        return -1;
    }

    CPE_INFO(nmgr->m_em, "gd_net_mgr listen start success!");
    
    return 0;
}

int gd_net_mgr_create_controler_client(gd_net_mgr_t nmgr) {
    struct sockaddr_in addr;
    socklen_t len;

    if(getsockname(nmgr->m_control_fd_listen, (struct sockaddr *)&addr, &len) != 0) {
        CPE_ERROR(nmgr->m_em, "gd_net_mgr get listen port error, error=%s\n", strerror(errno));
        close(nmgr->m_control_fd_listen);
        nmgr->m_control_fd_listen = -1;
        return -1;
    }

    return 0;
}

int gd_net_mgr_create_controler(gd_net_mgr_t nmgr) {
    if (gd_net_mgr_create_controler_listener(nmgr) != 0) {
        return -1;
    }

    if (gd_net_mgr_create_controler_client(nmgr) != 0) {
        return -1;
    }

    return 0;
}

