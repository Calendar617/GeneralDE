#ifndef GD_NET_SERVICE_TCP_H
#define GD_NET_SERVICE_TCP_H
#include "net_service.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_svr_t
gd_net_svr_create_tcp(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    const char * ip,
    short port);

const char * gd_net_svr_tcp_ip(gd_net_svr_t svr);
short gd_net_svr_tcp_port(gd_net_svr_t svr);

#ifdef __cplusplus
}
#endif

#endif
