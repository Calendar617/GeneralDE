#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_connector.h"
#include "gd/net/net_endpoint.h"
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

    connector->m_ep = NULL;
    cpe_hash_entry_init(&connector->m_hh);

    if (cpe_hash_table_insert_unique(&nmgr->m_connectors, connector) != 0) {
        CPE_ERROR(nmgr->m_em, "connector %s is already exist!", name);
        mem_free(nmgr->m_alloc, buf);
        return NULL;
    }
    return connector;
}

void gd_net_connector_free(gd_net_connector_t connector) {
    assert(connector);
    assert(connector->m_mgr);

    if (connector->m_ep) {
        gd_net_ep_free(connector->m_ep);
        assert(connector->m_ep == NULL);
    }

    cpe_hash_table_remove_by_ins(&connector->m_mgr->m_connectors, connector);
    mem_free(connector->m_mgr->m_alloc, (void*)connector->m_name);
}

gd_net_connector_t
gd_net_connector_create_with_ep(
    gd_net_mgr_t nmgr,
    const char * name,
    const char * ip,
    short port,
    gd_net_chanel_t chanel_r,
    gd_net_chanel_t chanel_w)
{
    gd_net_connector_t connector;
    gd_net_ep_t ep;

    connector = gd_net_connector_create(nmgr, name, ip, port);
    if (connector == NULL) return NULL;

    ep = gd_net_ep_create(nmgr, chanel_r, chanel_w);
    if (ep == NULL) {
        gd_net_connector_free(connector);
        return NULL;
    }

    if (gd_net_connector_bind(connector, ep) != 0) {
        gd_net_connector_free(connector);
        gd_net_ep_free(ep);
        return NULL;
    }

    return connector;
}

gd_net_connector_t
gd_net_connector_find(gd_net_mgr_t nmgr, const char * name) {
    struct gd_net_connector key;

    key.m_name = name;
    return (gd_net_connector_t)cpe_hash_table_find(&nmgr->m_connectors, &key);
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

uint32_t gd_net_connector_hash(gd_net_connector_t connector) {
    return cpe_hash_str(connector->m_name, strlen(connector->m_name));
}

int gd_net_connector_cmp(gd_net_connector_t l, gd_net_connector_t r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void gd_net_connectors_free(gd_net_mgr_t nmgr) {
    struct cpe_hash_it hashIt;
    gd_net_connector_t pre;
    gd_net_connector_t cur;

    cpe_hash_it_init(&hashIt, &nmgr->m_connectors);

    pre = NULL;
    while((cur = (gd_net_connector_t)cpe_hash_it_next(&hashIt))) {
        if (pre) gd_net_connector_free(pre);
        pre = cur;
    }

    if (pre) gd_net_connector_free(pre);
}
