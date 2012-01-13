#ifndef GD_DP_IMPL_INTERNAL_OPS_H
#define GD_DP_IMPL_INTERNAL_OPS_H
#include "dp_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t gd_dp_rsp_hash(const gd_dp_rsp_t rsp);
int gd_dp_rsp_cmp(const gd_dp_rsp_t l, const gd_dp_rsp_t r);
void gd_dp_rsp_free_i(gd_dp_rsp_t rsp);

/*binding operations*/
void gd_dp_binding_free(struct gd_dp_binding * binding);
void gd_dp_binding_free_i(struct gd_dp_binding * binding);

int32_t gd_dp_binding_hash(const struct gd_dp_binding * rsp);
int gd_dp_binding_cmp(const struct gd_dp_binding * l, const struct gd_dp_binding * r);

#ifdef __cplusplus
}
#endif

#endif
