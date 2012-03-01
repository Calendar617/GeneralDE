#ifndef CPE_NET_TYPES_H
#define CPE_NET_TYPES_H
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    net_ep_file = 0
    , net_ep_socket = 1
} net_ep_type_t;

typedef enum {
    net_connector_state_disable = -1
    , net_connector_state_idle = 0
    , net_connector_state_connecting
    , net_connector_state_connected
    , net_connector_state_error
} net_connector_state_t;

typedef uint32_t net_ep_id_t;

typedef struct net_mgr * net_mgr_t;
typedef struct net_chanel * net_chanel_t;
typedef struct net_ep * net_ep_t;
typedef struct net_listener * net_listener_t;
typedef struct net_connector * net_connector_t;

typedef void (*net_accept_fun_t)(net_listener_t listener, net_ep_t ep, void * ctx);

typedef void (*net_connector_state_monitor_fun_t)(net_connector_t connector, void * ctx);

#ifdef __cplusplus
}
#endif

#endif
