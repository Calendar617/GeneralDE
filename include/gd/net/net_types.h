#ifndef GD_NET_TYPES_H
#define GD_NET_TYPES_H
#include "cpe/dr/dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_net_mgr * gd_net_mgr_t;
typedef struct gd_net_chanel * gd_net_chanel_t;
typedef struct gd_net_svr * gd_net_svr_t;

typedef enum  gd_net_svr_type {
    gd_net_svr_type_tcp
} gd_net_svr_type_t;

#ifdef __cplusplus
}
#endif

#endif
