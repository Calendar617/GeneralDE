#ifndef USF_BPG_NET_INTERNAL_TYPES_H
#define USF_BPG_NET_INTERNAL_TYPES_H
#include "cpe/utils/hash.h"
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
    struct mem_buffer m_rsp_buf;

    size_t m_read_chanel_size;
    size_t m_write_chanel_size;

    dp_rsp_t m_reply_rsp;

    struct cpe_hash_table m_cliensts;
    struct cpe_hash_table m_connections;

    int m_debug;
};

struct bpg_net_agent_binding {
    uint64_t m_client_id;
    uint32_t m_connection_id;

    struct cpe_hash_entry m_hh_client;
    struct cpe_hash_entry m_hh_connection;
};

struct bpg_net_client {
    gd_app_context_t m_app;
    bpg_pkg_manage_t m_pkg_manage;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;
    net_connector_t m_connector;

    size_t m_req_max_size;
    bpg_pkg_t m_req_buf;
    struct mem_buffer m_send_encode_buf;

    bpg_pkg_dsp_t m_rsp_dsp;

    dp_rsp_t m_send_rsp;

    int m_debug;
};

typedef enum bpg_net_pkg_next_step {
    bpg_net_pkg_next_go_with_connection_id
    , bpg_net_pkg_next_go_without_connection_id
    , bpg_net_pkg_next_ignore
    , bpg_net_pkg_next_close
} bpg_net_pkg_next_step_t;

#ifdef __cplusplus
}
#endif

#endif
