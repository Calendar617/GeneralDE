#ifndef GD_DP_IMPL_INTERNAL_OPS_H
#define GD_DP_IMPL_INTERNAL_OPS_H
#include "dp_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t gd_dp_rsp_hash(const gd_dp_rsp_t rsp);
int gd_dp_rsp_cmp(const gd_dp_rsp_t l, const gd_dp_rsp_t r);
void gd_dp_rsp_free_i(gd_dp_rsp_t rsp);

/*processing buf operations*/
void gd_dp_pbuf_init(gd_dp_mgr_t dm, struct gd_dp_processing_rsp_buf * buf);
void gd_dp_pbuf_fini(gd_dp_mgr_t dm, struct gd_dp_processing_rsp_buf * buf);
void gd_dp_pbuf_add_rsp(struct gd_dp_processing_rsp_buf * buf, gd_dp_rsp_t rsp, error_monitor_t em);
void gd_dp_pbuf_remove_rsp(gd_dp_mgr_t dm, gd_dp_rsp_t rsp);
gd_dp_rsp_t gd_dp_pbuf_retrieve_first(struct gd_dp_processing_rsp_buf * buf);

/*binding operations*/
void gd_dp_binding_free(struct gd_dp_binding * binding);
void gd_dp_binding_free_i(struct gd_dp_binding * binding);

int32_t gd_dp_binding_hash(const struct gd_dp_binding * rsp);
int gd_dp_binding_cmp(const struct gd_dp_binding * l, const struct gd_dp_binding * r);

#ifdef __cplusplus
}
#endif

#endif
