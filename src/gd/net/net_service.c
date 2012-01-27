#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "gd/net/net_service.h"
#include "net_internal_ops.h"

gd_net_svr_t
gd_net_svr_crate_i(
    gd_net_mgr_t nmgr,
    gd_net_svr_type_t type,
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    size_t total_capacity)
{
    gd_net_svr_t svr;
    size_t nameLen;
    char * buf;

    assert(nmgr);

    buf = (char *)mem_alloc(nmgr->m_alloc, nameLen + 1 + total_capacity);
    if (buf == NULL) return NULL;

    memcpy(buf, name,  nameLen + 1);
    svr = (gd_net_svr_t)(buf + nameLen + 1);

    svr->m_mgr = nmgr;
    svr->m_type = type;
    svr->m_name = (char *)buf;
    svr->m_chanel_read = readChanel;
    svr->m_chanel_write = writeChanel;

    TAILQ_INSERT_TAIL(&nmgr->m_svrs, svr, m_svr_next);

    return svr;
}

void gd_net_svr_free(gd_net_svr_t svr) {
    assert(svr);

    TAILQ_REMOVE(&svr->m_mgr->m_svrs, svr, m_svr_next);

    mem_free(svr->m_mgr->m_alloc, svr->m_name);
}

gd_net_svr_type_t
gd_net_svr_type(gd_net_svr_t svr) {
    return svr->m_type;
}

const char *
gd_net_svr_name(gd_net_svr_t svr) {
    return svr->m_name;
}

gd_net_chanel_t
gd_net_svr_chanel_read(gd_net_svr_t svr) {
    return svr->m_chanel_read;
}

gd_net_chanel_t
gd_net_svr_chanel_write(gd_net_svr_t svr) {
    return svr->m_chanel_write;
}

