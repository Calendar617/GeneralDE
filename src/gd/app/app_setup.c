#include "cpe/pal/pal_external.h"
#include "cpe/net/net_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "gd/app/app_context.h"

EXPORT_DIRECTIVE
int app_setup_app_init(gd_app_context_t app, gd_app_module_t module, cfg_t cfg) {
    net_mgr_set_debug(gd_app_net_mgr(app), cfg_get_int16(cfg, "debug-net", 0));
    return 0;
}

EXPORT_DIRECTIVE
void app_setup_app_fini(gd_app_context_t app, gd_app_module_t module) {
}

