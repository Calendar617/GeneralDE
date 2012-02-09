#ifndef GD_NET_CHANEL_H
#define GD_NET_CHANEL_H
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_chanel_t
gd_net_chanel_mem_create(gd_net_mgr_t nmgr, size_t capacity);

void gd_net_chanel_free(gd_net_chanel_t chanel);

size_t gd_net_chanel_capacity(gd_net_chanel_t chanel);
int gd_net_chanel_wite(gd_net_chanel_t chanel, void * buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
