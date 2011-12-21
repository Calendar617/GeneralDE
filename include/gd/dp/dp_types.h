#ifndef GD_DP_TYPES_H
#define GD_DP_TYPES_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "cpe/pal/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_dp_node * gd_dp_node_t;
typedef struct gd_dp_rsp * gd_dp_rsp_t;
typedef struct gd_dp_rsp_it * gd_dp_rsp_it_t;
typedef struct gd_dp_req * gd_dp_req_t;
typedef struct gd_dp_mgr * gd_dp_mgr_t;
typedef struct gd_dp_rsp_type * gd_dp_rsp_type_t;

typedef int (*gd_dp_rsp_process_fun_t)(gd_dp_req_t req, void * ctx, error_monitor_t em);

struct gd_dp_rsp_type {
    const char * name;
    void (*destruct)(gd_dp_rsp_t rsp);
};

typedef gd_dp_rsp_t (*gd_dp_rsp_it_next_fun)(gd_dp_rsp_it_t it);

struct gd_dp_rsp_it {
    gd_dp_rsp_it_next_fun m_next_fun;
    void * m_context;
};

#ifdef __cplusplus
}
#endif

#endif


