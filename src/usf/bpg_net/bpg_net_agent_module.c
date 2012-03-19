#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cvt.h"
#include "cpe/net/net_listener.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/bpg/bpg_req.h"
#include "usf/bpg_net/bpg_net_agent.h"
#include "bpg_net_internal_ops.h"

EXPORT_DIRECTIVE
int bpg_net_agent_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    bpg_net_agent_t bpg_net_agent;
    const char * ip;
    short port;
    int accept_queue_size;

    ip = cfg_get_string(cfg, "ip", "");
    port = cfg_get_int16(cfg, "port", 0);
    accept_queue_size = cfg_get_int32(cfg, "accept-queue-size", 256);

    bpg_net_agent =
        bpg_net_agent_create(
            app, gd_app_module_name(module),
            ip, port, accept_queue_size,
            gd_app_alloc(app), gd_app_em(app));
    if (bpg_net_agent == NULL) return -1;

    bpg_net_agent->m_req_max_size =
        cfg_get_uint32(cfg, "req-max-size", bpg_net_agent->m_req_max_size);

    if (bpg_net_agent_set_cvt(
            bpg_net_agent,
            cfg_get_string(cfg, "basepkg-cvt", NULL)) != 0)
    {
        CPE_ERROR(
            gd_app_em(app),
            "%s: create: set basepkg-cvt=%s fail",
            gd_app_module_name(module),
            cfg_get_string(cfg, "basepkg-cvt", NULL));
        bpg_net_agent_free(bpg_net_agent);
        return -1;
    }

    bpg_net_agent->m_debug = cfg_get_int32(cfg, "debug", 0);

    if (bpg_net_agent->m_debug) {
        CPE_INFO(
            gd_app_em(app),
            "%s: create: done. ip=%s, port=%u, accept-queue-size=%d, req-max-size=%d, basepkg-cvt=%s",
            gd_app_module_name(module),
            ip, bpg_net_agent_port(bpg_net_agent),
            accept_queue_size, (int)bpg_net_agent->m_req_max_size,
            bpg_net_agent_cvt_name(bpg_net_agent));
    }

    return 0;
}

EXPORT_DIRECTIVE
void bpg_net_agent_app_fini(gd_app_context_t app, gd_app_module_t module) {
    bpg_net_agent_t bpg_net_agent;

    bpg_net_agent = bpg_net_agent_find_nc(app, gd_app_module_name(module));
    if (bpg_net_agent) {
        bpg_net_agent_free(bpg_net_agent);
    }
}
