#ifndef GD_NET_SERVICE_TCP_H
#define GD_NET_SERVICE_TCP_H
#include "net_service.h"

#ifdef __cplusplus
extern "C" {
#endif

/*tcp client operations*/
gd_net_svr_t
gd_net_svr_create_tcp_client(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    const char * target_ip,
    short target_port);

const char * gd_net_svr_tcp_client_target_ip(gd_net_svr_t svr);
short gd_net_svr_tcp_client_target_port(gd_net_svr_t svr);

/*tcp server operations*/
gd_net_svr_t
gd_net_svr_create_tcp_server(
    gd_net_mgr_t nmgr, 
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    const char * ip,
    short port);

const char * gd_net_svr_tcp_server_ip(gd_net_svr_t svr);
short gd_net_svr_tcp_server_port(gd_net_svr_t svr);

#ifdef __cplusplus
}
#endif

#endif
