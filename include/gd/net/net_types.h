#ifndef GD_NET_TYPES_H
#define GD_NET_TYPES_H
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_net_mgr * gd_net_mgr_t;
typedef struct gd_net_chanel * gd_net_chanel_t;
typedef struct gd_net_svr * gd_net_svr_t;

typedef void (*gd_net_svr_process_fun_t)(gd_net_svr_t svr);

typedef enum  gd_net_svr_type {
    gd_net_svr_type_local
    , gd_net_svr_type_tcp_client
    , gd_net_svr_type_tcp_listener
    , gd_net_svr_type_tcp_server
} gd_net_svr_type_t;

typedef enum gd_net_svr_state {
    gd_net_svr_state_init
    , gd_net_svr_state_starting
    , gd_net_svr_state_runing
    , gd_net_svr_state_shutingdown
} gd_net_svr_state_t;

typedef enum gd_net_svr_close_op {
    gd_net_svr_close_op_leave
    , gd_net_svr_close_op_resterat
    , gd_net_svr_close_op_free
} gd_net_svr_close_op_t;

#ifdef __cplusplus
}
#endif

#endif
