#ifndef USF_BPG_NET_INTERNAL_TYPES_H
#define USF_BPG_NET_INTERNAL_TYPES_H
#include "cpe/net/net_types.h"
#include "usf/bpg_net/bpg_net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bpg_net_agent {
    gd_app_context_t m_app;
    bpg_pkg_manage_t m_pkg_manage;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    net_listener_t m_listener;

    size_t m_req_max_size;
    bpg_pkg_t m_req_buf;

    gd_dp_rsp_t m_reply_rsp;

    int m_debug;
};

struct bpg_net_client {
    gd_app_context_t m_app;
    bpg_pkg_manage_t m_pkg_manage;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    net_connector_t m_connector;

    size_t m_req_max_size;
    bpg_pkg_t m_req_buf;

    int m_debug;
};

#ifdef __cplusplus
}
#endif

#endif
