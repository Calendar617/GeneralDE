#ifndef GD_NET_SERVICE_H
#define GD_NET_SERVICE_H
#include "cpe/utils/memory.h"
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_svr_t gd_net_svr_find(gd_net_mgr_t nmgr, const char * name);
void gd_net_svr_free(gd_net_svr_t svr);

const char * gd_net_svr_name(gd_net_svr_t svr);
gd_net_svr_type_t gd_net_svr_type(gd_net_svr_t svr);
gd_net_chanel_t gd_net_svr_chanel_read(gd_net_svr_t svr);
gd_net_chanel_t gd_net_svr_chanel_write(gd_net_svr_t svr);

#ifdef __cplusplus
}
#endif

#endif
