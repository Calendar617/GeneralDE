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
    svr->m_close_op = gd_net_svr_close_op_leave;
    svr->m_state = gd_net_svr_state_init;
    svr->m_name = (char *)buf;
    svr->m_chanel_read = readChanel;
    svr->m_chanel_write = writeChanel;

    gd_net_svr_enqueue(svr);

    return svr;
}

void gd_net_svr_free_i(gd_net_svr_t svr) {
    gd_net_svr_dequeue(svr);
    mem_free(svr->m_mgr->m_alloc, svr->m_name);
}

void gd_net_svr_free(gd_net_svr_t svr) {
    assert(svr);

    g_net_svr_ops[svr->m_type].m_free(svr);
}

gd_net_svr_type_t
gd_net_svr_type(gd_net_svr_t svr) {
    return svr->m_type;
}

gd_net_svr_state_t
gd_net_svr_state(gd_net_svr_t svr) {
    return svr->m_state;
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

struct gd_net_svr_op g_net_svr_ops[] = {
        /*gd_net_svr_type_local*/
    {   /*free*/ NULL
      , /*init*/ NULL
      , /*fini*/ NULL
    }
    ,   /*gd_net_svr_type_tcp_client*/
    {   /*free*/ gd_net_svr_free_tcp_client
      , /*init*/ NULL
      , /*fini*/ NULL
    }
    ,   /*gd_net_svr_type_tcp_listener*/
    {   /*free*/ NULL
      , /*init*/ NULL
      , /*fini*/ NULL
    }
    ,   /*gd_net_svr_type_tcp_server*/
    {   /*free*/ NULL
      , /*init*/ NULL
      , /*fini*/ NULL
    }
};
