#ifndef GD_NET_INTERNAL_TYPES_H
#define GD_NET_INTERNAL_TYPES_H
#include "ev.h"
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/range.h"
#include "cpe/utils/hash.h"
#include "cpe/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(net_chanel_list, net_chanel) net_chanel_list_t;

#define GD_NET_EP_INVALID_ID ((net_ep_id_t)-1)
#define GD_NET_EP_COUNT_PER_PAGE (256)
#define GD_NET_EP_PAGE_INC (16)
struct net_ep_page;

struct net_mgr {
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;

    struct cpe_range_mgr m_ep_ids;
    size_t m_ep_page_capacity;
    struct net_ep_page * * m_ep_pages;

    net_chanel_list_t m_chanels;

    struct cpe_hash_table m_listeners;
    struct cpe_hash_table m_connectors;

    struct ev_loop * m_ev_loop;
};

struct net_listener {
    net_mgr_t m_mgr;
    char const * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    int m_acceptQueueSize;
    net_accept_fun_t m_acceptor_fun;
    void * m_acceptor_ctx;
    int m_fd;
    struct ev_io m_watcher;

    struct cpe_hash_entry m_hh;
};

struct net_connector {
    net_mgr_t m_mgr;
    char const * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    net_ep_t m_ep;
    net_connector_state_t m_state;
    net_connector_state_monitor_fun_t m_monitor_fun;
    void * m_monitor_ctx;
    struct ev_timer m_timer;
    struct cpe_hash_entry m_hh;
};

struct net_chanel {
    net_mgr_t m_mgr;
    void * m_buf;
    size_t m_capacity;
    void * m_write_ptr;
    void * m_read_ptr;
    void (*close)(net_chanel_t chanel);

    TAILQ_ENTRY(net_chanel) m_next;
};

struct net_ep {
    net_ep_id_t m_id;
    net_mgr_t m_mgr;
    net_ep_type_t m_type;
    struct net_chanel * m_chanel_r;
    struct net_chanel * m_chanel_w;
    struct net_connector * m_connector;
    int m_fd;
    struct ev_io m_watcher;
};

struct net_ep_page {
    struct net_ep m_eps[GD_NET_EP_COUNT_PER_PAGE];
};

#ifdef __cplusplus
}
#endif

#endif