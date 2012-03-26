#ifndef CPE_DP_TYPES_H
#define CPE_DP_TYPES_H
#include "cpe/pal/pal_types.h"
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dp_node_type * dp_node_type_t;
typedef struct dp_node * dp_node_t;
typedef struct dp_rsp * dp_rsp_t;
typedef struct dp_rsp_it * dp_rsp_it_t;
typedef struct dp_req * dp_req_t;
typedef struct dp_mgr * dp_mgr_t;
typedef struct dp_rsp_type * dp_rsp_type_t;

typedef int (*dp_node_send_fun_t)(dp_node_t node, dp_req_t req, error_monitor_t em);
typedef int (*dp_node_reply_fun_t)(dp_node_t node, dp_req_t req, char * buf, size_t size, error_monitor_t em);

struct dp_node_type {
    const char * name;
    void (*destruct)(dp_node_t node);
    dp_node_send_fun_t send;
    dp_node_reply_fun_t reply;
};

typedef int (*dp_rsp_process_fun_t)(dp_req_t req, void * ctx, error_monitor_t em);

struct dp_rsp_type {
    const char * name;
    void (*destruct)(dp_rsp_t rsp);
};

typedef dp_rsp_t (*dp_rsp_it_next_fun)(dp_rsp_it_t it);

struct dp_rsp_it {
    dp_rsp_it_next_fun m_next_fun;
    void * m_context;
};

#ifdef __cplusplus
}
#endif

#endif


