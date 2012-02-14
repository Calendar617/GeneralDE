#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_listener.h"
#include "net_internal_ops.h"

static
int gd_net_listener_listen(gd_net_listener_t listener) {
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: socket call fail, errno=%s!", listener->m_name, strerror(errno));
        return -1;
    }

    if(bind(fd, (struct sockaddr *)&listener->m_addr, sizeof(listener->m_addr)) == -1 ) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: bind error, errno=%s", listener->m_name, strerror(errno));
        gd_net_socket_close(&fd, listener->m_mgr->m_em);
        return -1;
    }

    if (listen(fd, listener->m_acceptQueueSize) == -1) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: listen error, errno=%s", listener->m_name, strerror(errno));
        gd_net_socket_close(&fd, listener->m_mgr->m_em);
        return -1;
    }

    if (gd_net_socket_set_none_block(fd, listener->m_mgr->m_em)) {
        gd_net_socket_close(&fd, listener->m_mgr->m_em);
        return -1;
    }

    CPE_INFO(listener->m_mgr->m_em, "listener %s: listen start", listener->m_name);
    return fd;
}

gd_net_listener_t
gd_net_listener_create(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port,
    int acceptQueueSize,
    gd_net_accept_fun acceptor)
{
    gd_net_listener_t listener;
    char * buf;
    size_t nameLen;
    int fd;

    nameLen = strlen(name);

    buf = mem_alloc(nmgr->m_alloc, sizeof(struct gd_net_listener) + nameLen + 1);
    if (buf == NULL) return NULL;

    memcpy(buf, name, nameLen + 1);
    listener = (gd_net_listener_t)(buf + nameLen + 1);

    listener->m_mgr = nmgr;
    listener->m_name = buf;

    /*设置客户端连接端口*/
    struct sockaddr_in * inetAddr = (struct sockaddr_in *)(&listener->m_addr);
    inetAddr->sin_family = AF_INET;
    inetAddr->sin_port = port;
    inetAddr->sin_addr.s_addr = inet_addr(ip);

    listener->m_acceptQueueSize = acceptQueueSize;

    fd = gd_net_listener_listen(listener);
    if (fd < 0) {
        mem_free(nmgr->m_alloc, buf);
        return 0;
    }

    
    return listener;
}
