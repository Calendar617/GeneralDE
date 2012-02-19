#ifndef GD_NET_INTERNAL_TYPES_H
#define GD_NET_INTERNAL_TYPES_H
#include "ev.h"
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/range.h"
#include "cpe/utils/hash.h"
#include "gd/tl/tl_manage.h"
#include "gd/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(gd_net_chanel_list, gd_net_chanel) gd_net_chanel_list_t;

#define GD_NET_EP_INVALID_ID ((gd_net_ep_id_t)-1)
#define GD_NET_EP_COUNT_PER_PAGE (256)
#define GD_NET_EP_PAGE_INC (16)
struct gd_net_ep_page;

struct gd_net_mgr {
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;

    struct cpe_range_mgr m_ep_ids;
    size_t m_ep_page_capacity;
    struct gd_net_ep_page * * m_ep_pages;

    gd_net_chanel_list_t m_chanels;

    struct cpe_hash_table m_listeners;
    struct cpe_hash_table m_connectors;

    struct ev_loop * m_ev_loop;
};

struct gd_net_listener {
    gd_net_mgr_t m_mgr;
    char const * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    int m_acceptQueueSize;
    gd_net_accept_fun_t m_acceptor_fun;
    void * m_acceptor_ctx;
    int m_fd;
    struct ev_io m_watcher;

    struct cpe_hash_entry m_hh;
};

struct gd_net_connector {
    gd_net_mgr_t m_mgr;
    char const * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    gd_net_ep_t m_ep;
    gd_net_connector_state_t m_state;
    gd_net_connector_state_monitor_fun_t m_monitor_fun;
    void * m_monitor_ctx;
    struct ev_timer m_timer;
    struct cpe_hash_entry m_hh;
};

struct gd_net_chanel {
    gd_net_mgr_t m_mgr;
    void * m_buf;
    size_t m_capacity;
    void * m_write_ptr;
    void * m_read_ptr;
    void (*close)(gd_net_chanel_t chanel);

    TAILQ_ENTRY(gd_net_chanel) m_next;
};

struct gd_net_ep {
    gd_net_ep_id_t m_id;
    gd_net_mgr_t m_mgr;
    gd_net_ep_type_t m_type;
    struct gd_net_chanel * m_chanel_r;
    struct gd_net_chanel * m_chanel_w;
    struct gd_net_connector * m_connector;
    int m_fd;
    struct ev_io m_watcher;
};

struct gd_net_ep_page {
    struct gd_net_ep m_eps[GD_NET_EP_COUNT_PER_PAGE];
};

#ifdef __cplusplus
}
#endif

#endif
