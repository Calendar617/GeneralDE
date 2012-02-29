#ifndef GD_VNET_ENDPOINT_H
#define GD_VNET_ENDPOINT_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "vnet_types.h"

#ifdef __cplusplus
extern "C" {
#endif

vnet_ep_t vnet_ep_create(vnet_manage_t mgr, const char * name, size_t capacity);
void vnet_ep_free(vnet_ep_t ep);

vnet_ep_t vnet_ep_find(vnet_manage_t mgr, cpe_hash_string_t name);
vnet_ep_t vnet_ep_find_nc(vnet_manage_t mgr, const char * name);

const char * vnet_ep_name(vnet_ep_t ep);
cpe_hash_string_t vnet_ep_name_hs(vnet_ep_t ep);
size_t vnet_ep_capacity(vnet_ep_t ep);
void * vnet_ep_data(vnet_ep_t ep);

void vnet_ep_set_dispatcher(
    vnet_ep_t ep,
    vnet_dispatch_fun_t fun,
    void * ctx);

int vnet_ep_send(vnet_ep_t ep, const void * buf, size_t size);
int vnet_ep_rece(vnet_ep_t ep, void * buf, size_t * size);
int vnet_ep_peek(vnet_ep_t ep, void * buf, size_t * size);
int vnet_ep_erase(vnet_ep_t ep, size_t size);

#ifdef __cplusplus
}
#endif

#endif
