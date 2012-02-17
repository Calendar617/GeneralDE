#ifndef GD_NET_INTERNAL_TYPES_H
#define GD_NET_INTERNAL_TYPES_H
#include "ev.h"
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/range.h"
#include "gd/tl/tl_manage.h"
#include "gd/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(gd_net_chanel_list, gd_net_chanel) gd_net_chanel_list_t;
typedef TAILQ_HEAD(gd_net_listener_list, gd_net_listener) gd_net_listener_list_t;
typedef TAILQ_HEAD(gd_net_connector_list, gd_net_connector) gd_net_connector_list_t;

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
    gd_net_listener_list_t m_listeners;
    gd_net_connector_list_t m_connectors;

    struct ev_loop * m_ev_loop;
};

struct gd_net_listener {
    gd_net_mgr_t m_mgr;
    char * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    int m_acceptQueueSize;
    int m_fd;
    struct ev_io m_watcher;

    TAILQ_ENTRY(gd_net_listener) m_next;
};

struct gd_net_connector {
    gd_net_mgr_t m_mgr;
    char * m_name;
    char m_addr[16]; /*sizeof(sockaddr)*/
    gd_net_ep_t m_ep;

    TAILQ_ENTRY(gd_net_connector) m_next;
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
    struct gd_net_chanel * m_chanel_read;
    struct gd_net_chanel * m_chanel_write;
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
