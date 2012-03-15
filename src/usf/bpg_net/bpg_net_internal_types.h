#ifndef USF_BPG_NET_INTERNAL_TYPES_H
#define USF_BPG_NET_INTERNAL_TYPES_H
#include "usf/bpg_net/bpg_net_types.h"
#include "cpe/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_net_agent {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    net_listener_t m_listener;
    int m_debug;
};

#ifdef __cplusplus
}
#endif

#endif
