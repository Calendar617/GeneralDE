#ifndef GD_NET_INTERNAL_TYPES_H
#define GD_NET_INTERNAL_TYPES_H
#include "cpe/pal/pal_queue.h"
#include "cpe/utils/memory.h"
#include "gd/net/net_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef TAILQ_HEAD(gd_net_chanel_list, gd_net_chanel) gd_net_chanel_list_t;
typedef TAILQ_HEAD(gd_net_svr_list, gd_net_svr) gd_net_svr_list_t;

struct gd_net_mgr {
    mem_allocrator_t m_alloc;
    gd_net_chanel_list_t m_chanels;
    gd_net_svr_list_t m_svrs;
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

#define GD_NET_SVR_HEAD                         \
    gd_net_mgr_t m_mgr;                         \
    gd_net_svr_type_t m_type;                   \
    char * m_name;                              \
    struct gd_net_chanel * m_chanel_read;       \
    struct gd_net_chanel * m_chanel_write;      \
    TAILQ_ENTRY(gd_net_svr) m_svr_next;         \

struct gd_net_svr {
    GD_NET_SVR_HEAD
};

struct gd_net_svr_tcp {
    GD_NET_SVR_HEAD
    char m_ip[32];
    short m_port;
};

#ifdef __cplusplus
}
#endif

#endif
