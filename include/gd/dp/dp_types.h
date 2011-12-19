#ifndef GD_DP_TYPES_H
#define GD_DP_TYPES_H
#include "cpe/utils/memory.h"
#include "cpe/pal/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gd_dp_node * gd_dp_node_t;
typedef struct gd_dp_rsp * gd_dp_rsp_t;
typedef struct gd_dp_req * gd_dp_req_t;
typedef struct gd_dp_mgr * gd_dp_mgr_t;

typedef void (*gd_dp_rsp_process_fun_t)(gd_dp_req_t req, void * ctx);

#ifdef __cplusplus
}
#endif

#endif


