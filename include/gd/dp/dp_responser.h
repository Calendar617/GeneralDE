#ifndef GD_DP_RESPONSER_H
#define GD_DP_RESPONSER_H
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_rsp_t gd_dp_rsp_create(gd_dp_mgr_t dp);
void gd_dp_rsp_free(gd_dp_rsp_t rsp);

enum gd_dp_rsp_ot {
    gd_dp_rsp_set_processor
} gd_dp_rsp_op_t ;

#ifdef __cplusplus
}
#endif

#endif
