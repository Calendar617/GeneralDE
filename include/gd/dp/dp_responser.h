#ifndef GD_DP_RESPONSER_H
#define GD_DP_RESPONSER_H
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_rsp_t gd_dp_rsp_create(gd_dp_mgr_t dp, const char * name);
void gd_dp_rsp_free(gd_dp_rsp_t rsp);

const char * gd_dp_rsp_name(gd_dp_rsp_t rsp);
gd_dp_rsp_process_fun_t gd_dp_rsp_processor(gd_dp_rsp_t rsp);

typedef enum gd_dp_rsp_opt {
    gd_dp_rsp_set_processor /*see gd_dp_rsp_process_fun_t*/
    , gd_dp_rsp_set_context
} gd_dp_rsp_opt_t ;

int gd_dp_rsp_set_opt(gd_dp_rsp_t rsp, gd_dp_rsp_opt_t opt, ...);

#ifdef __cplusplus
}
#endif

#endif
