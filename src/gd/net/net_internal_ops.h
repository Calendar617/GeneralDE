#ifndef GD_NET_INTERNAL_OPS_H
#define GD_NET_INTERNAL_OPS_H
#include "net_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_net_svr_t
gd_net_svr_crate_i(
    gd_net_mgr_t nmgr, 
    gd_net_svr_type_t type,
    const char * name,
    gd_net_chanel_t readChanel,
    gd_net_chanel_t writeChanel,
    size_t total_capacity);

void gd_net_svr_free_i(gd_net_svr_t svr);

int gd_net_svr_fd_add(
    gd_net_mgr_t nmgr,
    gd_net_svr_t svr,
    int fd);

void gd_net_svr_fd_remove(
    gd_net_mgr_t nmgr,
    int fd);

void gd_net_svr_update_state(
    gd_net_svr_t svr,
    gd_net_svr_state_t state);

typedef struct gd_net_svr_op {
    void (*m_free)(gd_net_svr_t svr);
    void (*m_init)(gd_net_svr_t svr);
    void (*m_fini)(gd_net_svr_t svr);
} * gd_net_svr_op_t;

extern struct gd_net_svr_op g_net_svr_ops[];

/*mgr control ops*/
int gd_net_mgr_create_controler(gd_net_mgr_t nmgr);

/*service local operations*/

/*service tcp client operations*/
void gd_net_svr_free_tcp_client(gd_net_svr_t svr);
gd_net_svr_state_t gd_net_svr_init_tcp_client(gd_net_svr_t svr);
void gd_net_svr_fini_tcp_client(gd_net_svr_t svr);

/*tcp ops*/
void gd_net_socket_close(int * fd, error_monitor_t em);
int gd_net_socket_set_none_block(int fd, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
