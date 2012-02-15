#ifndef GD_NET_INTERNAL_TYPES_H
#define GD_NET_INTERNAL_TYPES_H
#include "ev.h"
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "gd/tl/tl_manage.h"
#include "gd/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(gd_net_chanel_list, gd_net_chanel) gd_net_chanel_list_t;
typedef TAILQ_HEAD(gd_net_ep_list, gd_net_ep) gd_net_ep_list_t;
typedef TAILQ_HEAD(gd_net_listener_list, gd_net_listener) gd_net_listener_list_t;

struct gd_net_mgr {
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;

    gd_net_chanel_list_t m_chanels;
    gd_net_ep_list_t m_endpoints;
    gd_net_listener_list_t m_listeners;

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

struct gd_net_chanel {
    gd_net_mgr_t m_mgr;
    void * m_buf;
    size_t m_capacity;
    void * m_write_ptr;
    void * m_read_ptr;
    void (*close)(gd_net_chanel_t chanel);

    TAILQ_ENTRY(gd_net_chanel) m_next;
};

#define GD_NET_EP_HEAD                          \
    gd_net_mgr_t m_mgr;                         \
    struct gd_net_chanel * m_chanel_read;       \
    struct gd_net_chanel * m_chanel_write;      \
    TAILQ_ENTRY(gd_net_ep) m_ep_next;           \

struct gd_net_ep {
    GD_NET_EP_HEAD
};

struct gd_net_ep_tcp {
    GD_NET_EP_HEAD
    char m_ip[32];
    short m_port;
    int m_fd;
    struct ev_io m_watcher;
};

#ifdef __cplusplus
}
#endif

#endif
