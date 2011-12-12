#ifndef GD_DP_MANAGE_H
#define GD_DP_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "gd/tl/tl_types.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_mgr_t gd_dp_mgr_create(mem_allocrator_t alloc);
void gd_dp_mgr_free(gd_dp_mgr_t dp);

/*bind command to rsp*/
int gd_dp_mgr_bind_numeric(gd_dp_rsp_t rsp, int32_t cmd, error_monitor_t em);
int gd_dp_mgr_unbind_numeric(gd_dp_mgr_t dp, int32_t cmd);

/*rsp find operations*/
gd_dp_rsp_t gd_dp_rsp_find_by_name(gd_dp_mgr_t dp, const char * name);
gd_dp_rsp_t gd_dp_rsp_find_by_numeric(gd_dp_mgr_t dp, int32_t cmd);
gd_dp_rsp_t gd_dp_rsp_find_by_string(gd_dp_mgr_t dp, const char * cmd);

#ifdef __cplusplus
}
#endif

#endif
