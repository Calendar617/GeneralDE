#ifndef GD_NET_TYPES_H
#define GD_NET_TYPES_H
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_net_mgr * gd_net_mgr_t;
typedef struct gd_net_chanel * gd_net_chanel_t;
typedef struct gd_net_ep * gd_net_ep_t;
typedef struct gd_net_listener * gd_net_listener_t;

typedef int (*gd_net_accept_fun)(gd_net_listener_t listener, gd_net_ep_t ep);

typedef void (*gd_net_svr_process_fun_t)(gd_net_ep_t svr);

#ifdef __cplusplus
}
#endif

#endif
