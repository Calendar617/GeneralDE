#ifndef GD_VNET_TYPES_H
#define GD_VNET_TYPES_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vnet_ep * vnet_ep_t;
typedef struct vnet_manage * vnet_manage_t;

typedef void (*vnet_dispatch_fun_t)(vnet_ep_t ep, void * ctx);

#ifdef __cplusplus
}
#endif

#endif
