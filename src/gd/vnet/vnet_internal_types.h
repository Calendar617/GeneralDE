#ifndef GD_VNET_INTERNAL_TYPES_H
#define GD_VNET_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "gd/vnet/vnet_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vnet_manage {
    mem_allocrator_t m_alloc;
    gd_app_context_t m_app;
};

struct vnet_ep {
    vnet_manage_t m_mgr;
    size_t m_capacity;
    vnet_dispatch_fun_t m_dispatcher_fun;
    void * m_dispatcher_ctx;
};

#ifdef __cplusplus
}
#endif

#endif
