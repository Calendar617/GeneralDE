#ifndef GD_NET_INTERNAL_OPS_H
#define GD_NET_INTERNAL_OPS_H
#include "net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_svr_t
gd_net_svr_crate_i(
    gd_net_mgr_t nmgr, 
    gd_net_svr_type_t type,
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    size_t total_capacity);


#ifdef __cplusplus
}
#endif

#endif
