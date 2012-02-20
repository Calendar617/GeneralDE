#ifndef GD_NET_TYPES_H
#define GD_NET_TYPES_H
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    gd_net_ep_file = 0
    , gd_net_ep_socket = 1
} gd_net_ep_type_t;

typedef enum {
    gd_net_connector_state_disable = -1
    , gd_net_connector_state_idle = 0
    , gd_net_connector_state_connecting
    , gd_net_connector_state_connected
    , gd_net_connector_state_error
} gd_net_connector_state_t;

typedef uint32_t gd_net_ep_id_t;

typedef struct gd_net_mgr * gd_net_mgr_t;
typedef struct gd_net_chanel * gd_net_chanel_t;
typedef struct gd_net_ep * gd_net_ep_t;
typedef struct gd_net_listener * gd_net_listener_t;
typedef struct gd_net_connector * gd_net_connector_t;

typedef void (*gd_net_accept_fun_t)(gd_net_listener_t listener, gd_net_ep_t ep, void * ctx);

typedef void (*gd_net_connector_state_monitor_fun_t)(gd_net_connector_t connector, void * ctx);

#ifdef __cplusplus
}
#endif

#endif
