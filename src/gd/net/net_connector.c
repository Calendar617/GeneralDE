#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_connector.h"
#include "net_internal_ops.h"

gd_net_connector_t
gd_net_connector_create(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port)
{
    gd_net_connector_t connector;
    char * buf;
    size_t nameLen;

    nameLen = strlen(name);

    buf = mem_alloc(nmgr->m_alloc, sizeof(struct gd_net_connector) + nameLen + 1);
    if (buf == NULL) return NULL;

    memcpy(buf, name, nameLen + 1);
    connector = (gd_net_connector_t)(buf + nameLen + 1);

    connector->m_mgr = nmgr;
    connector->m_name = buf;

    /*设置客户端连接端口*/
    struct sockaddr_in * inetAddr = (struct sockaddr_in *)(&connector->m_addr);
    inetAddr->sin_family = AF_INET;
    inetAddr->sin_port = port;
    inetAddr->sin_addr.s_addr = inet_addr(ip);

    TAILQ_INSERT_TAIL(&nmgr->m_connectors, connector, m_next);

    return connector;
}

void gd_net_connector_free(gd_net_connector_t connector) {
    assert(connector);
    assert(connector->m_mgr);

    TAILQ_REMOVE(&connector->m_mgr->m_connectors, connector, m_next);
    mem_free(connector->m_mgr->m_alloc, connector->m_name);
}

const char * gd_net_connector_name(gd_net_connector_t connector) {
    return connector->m_name;
}

int gd_net_connector_bind(gd_net_connector_t connector, gd_net_ep_t ep) {
    return 0;
}

int gd_net_connector_unbind(gd_net_connector_t connector) {
    return 0;
}
