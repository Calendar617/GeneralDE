#ifndef USF_BPG_NET_INTERNAL_TYPES_H
#define USF_BPG_NET_INTERNAL_TYPES_H
#include "cpe/net/net_types.h"
#include "usf/bpg/bpg_types.h"
#include "usf/bpg_net/bpg_net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_net_agent {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    bpg_manage_t m_bpg_manage;
    net_listener_t m_listener;
    dr_cvt_t m_cvt;

    size_t m_req_max_size;
    bpg_req_t m_req_buf;

    int m_debug;
};

struct bpg_net_client {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    net_connector_t m_connector;
    dr_cvt_t m_cvt;

    size_t m_req_max_size;
    bpg_req_t m_req_buf;

    int m_debug;
};

#ifdef __cplusplus
}
#endif

#endif
