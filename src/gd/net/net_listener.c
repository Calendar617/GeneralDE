#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_listener.h"
#include "gd/net/net_endpoint.h"
#include "net_internal_ops.h"

static
int gd_net_listener_listen(gd_net_listener_t listener) {
    listener->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listener->m_fd == -1) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: socket call fail, errno=%d (%s)!", listener->m_name, errno, strerror(errno));
        return -1;
    }

    if(bind(listener->m_fd, (struct sockaddr *)&listener->m_addr, sizeof(listener->m_addr)) == -1 ) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: bind error, errno=%d (%s)", listener->m_name, errno, strerror(errno));
        gd_net_socket_close(&listener->m_fd, listener->m_mgr->m_em);
        return -1;
    }

    if (listen(listener->m_fd, listener->m_acceptQueueSize) == -1) {
        CPE_ERROR(listener->m_mgr->m_em, "listener %s: listen error, errno=%d (%s)", listener->m_name, errno, strerror(errno));
        gd_net_socket_close(&listener->m_fd, listener->m_mgr->m_em);
        return -1;
    }

    if (gd_net_socket_set_none_block(listener->m_fd, listener->m_mgr->m_em)) {
        gd_net_socket_close(&listener->m_fd, listener->m_mgr->m_em);
        return -1;
    }

    CPE_INFO(listener->m_mgr->m_em, "listener %s: listen start", listener->m_name);
    return 0;
}

static void gd_net_listener_cb_accept(EV_P_ ev_io *w, int revents) {
    gd_net_listener_t listener;
    gd_net_ep_t ep;
    int new_fd;

    listener = w->data;
    assert(listener);

    new_fd = accept(listener->m_fd, 0, 0);
    if (new_fd == -1) {
        CPE_ERROR(
            listener->m_mgr->m_em,
            "listener %s: accept error, errno=%d (%s)",
            listener->m_name, errno, strerror(errno));
        return;
    }

    ep = gd_net_ep_create(listener->m_mgr, gd_net_ep_socket);
    if (ep == 0) {
        gd_net_socket_close(&new_fd, listener->m_mgr->m_em);
        return;
    }

    ep->m_fd = new_fd;

    listener->m_acceptor_fun(listener, ep, listener->m_acceptor_ctx);
}

gd_net_listener_t
gd_net_listener_create(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port,
    int acceptQueueSize,
    gd_net_accept_fun_t acceptor,
    void * acceptor_ctx)
{
    gd_net_listener_t listener;
    char * buf;
    size_t nameLen;

    nameLen = strlen(name);

    buf = mem_alloc(nmgr->m_alloc, sizeof(struct gd_net_listener) + nameLen + 1);
    if (buf == NULL) return NULL;

    memcpy(buf, name, nameLen + 1);
    listener = (gd_net_listener_t)(buf + nameLen + 1);

    listener->m_mgr = nmgr;
    listener->m_name = buf;
    cpe_hash_entry_init(&listener->m_hh);

    if (cpe_hash_table_insert_unique(&nmgr->m_listeners, listener) != 0) {
        CPE_ERROR(nmgr->m_em, "listener %s is already exist!", name);
        mem_free(nmgr->m_alloc, buf);
        return NULL;
    }

    /*设置客户端连接端口*/
    struct sockaddr_in * inetAddr = (struct sockaddr_in *)(&listener->m_addr);
    inetAddr->sin_family = AF_INET;
    inetAddr->sin_port = port;
    inetAddr->sin_addr.s_addr = 
        strcmp(ip, "") == 0
        ? INADDR_ANY
        : inet_addr(ip);
    if (inetAddr->sin_addr.s_addr == INADDR_NONE) {
        CPE_ERROR(nmgr->m_em, "listener %s address %s format error!", name, ip);
        cpe_hash_table_remove_by_ins(&nmgr->m_listeners, listener);
        mem_free(nmgr->m_alloc, buf);
        return NULL;
    }

    listener->m_acceptQueueSize = acceptQueueSize;
    listener->m_acceptor_fun = acceptor;
    listener->m_acceptor_ctx = acceptor_ctx;

    if (gd_net_listener_listen(listener) != 0) {
        cpe_hash_table_remove_by_ins(&nmgr->m_listeners, listener);
        mem_free(nmgr->m_alloc, buf);
        return 0;
    }

    listener->m_watcher.data = listener;
    ev_io_init(&listener->m_watcher, gd_net_listener_cb_accept, listener->m_fd, EV_READ);
    ev_io_start(nmgr->m_ev_loop, &listener->m_watcher);

    return listener;
}

void gd_net_listener_free(gd_net_listener_t listener) {
    assert(listener);
    assert(listener->m_mgr);

    cpe_hash_table_remove_by_ins(&listener->m_mgr->m_listeners, listener);
    ev_io_stop(listener->m_mgr->m_ev_loop, &listener->m_watcher);
    gd_net_socket_close(&listener->m_fd, listener->m_mgr->m_em);
    mem_free(listener->m_mgr->m_alloc, (void*)listener->m_name);
}

gd_net_listener_t
gd_net_listener_find(gd_net_mgr_t nmgr, const char * name) {
    struct gd_net_listener key;

    key.m_name = name;
    return (gd_net_listener_t)cpe_hash_table_find(&nmgr->m_listeners, &key);
}

short gd_net_listener_using_port(gd_net_listener_t listener) {
    struct sockaddr_in addr;
    socklen_t len;

    if (listener->m_fd < 0) return 0;

    if (((struct sockaddr_in *)(&listener->m_addr))->sin_port != 0) {
        return ((struct sockaddr_in *)(&listener->m_addr))->sin_port;
    }

    len = sizeof(addr);
    if (getsockname(listener->m_fd, (struct sockaddr *)&addr, &len) == -1) {
        CPE_ERROR(
            listener->m_mgr->m_em,
            "listener %s: getsockname fail, errno=%d (%s)!",
            listener->m_name, errno, strerror(errno));
        return 0;
    }

    return addr.sin_port;
}

uint32_t gd_net_listener_hash(gd_net_listener_t listener) {
    return cpe_hash_str(listener->m_name, strlen(listener->m_name));
}

int gd_net_listener_cmp(gd_net_listener_t l, gd_net_listener_t r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void gd_net_listeners_free(gd_net_mgr_t nmgr) {
    struct cpe_hash_it hashIt;
    gd_net_listener_t pre;
    gd_net_listener_t cur;

    cpe_hash_it_init(&hashIt, &nmgr->m_listeners);

    pre = NULL;
    while((cur = (gd_net_listener_t)cpe_hash_it_next(&hashIt))) {
        if (pre) gd_net_listener_free(pre);
        pre = cur;
    }

    if (pre) gd_net_listener_free(pre);
}
