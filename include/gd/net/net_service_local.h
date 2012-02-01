#ifndef GD_NET_SERVICE_LOCAL_H
#define GD_NET_SERVICE_LOCAL_H
#include "net_service.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_svr_t
gd_net_svr_create_local(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    gd_net_svr_process_fun_t);

#ifdef __cplusplus
}
#endif

#endif


