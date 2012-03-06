#ifndef CPE_NET_CHANEL_H
#define CPE_NET_CHANEL_H
#include "net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

net_chanel_t
net_chanel_mem_create(net_mgr_t nmgr, size_t capacity);

void net_chanel_free(net_chanel_t chanel);

size_t net_chanel_capacity(net_chanel_t chanel);
int net_chanel_wite(net_chanel_t chanel, void * buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
