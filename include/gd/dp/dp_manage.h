#ifndef GD_DP_MANAGE_H
#define GD_DP_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/hash_string.h"
#include "cpe/utils/error.h"
#include "cpe/cfg/cfg_types.h"
#include "gd/tl/tl_types.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_mgr_t gd_dp_mgr_create(mem_allocrator_t alloc);
void gd_dp_mgr_free(gd_dp_mgr_t dp);

/*bind command to rsp*/
int gd_dp_mgr_unbind_numeric(gd_dp_mgr_t dp, int32_t cmd);
int gd_dp_mgr_unbind_string(gd_dp_mgr_t dp, const char * cmd);

int gd_dp_rsp_bind_numeric(gd_dp_rsp_t rsp, int32_t cmd, error_monitor_t em);
int gd_dp_rsp_bind_string(gd_dp_rsp_t rsp, const char * cmd, error_monitor_t em);
int gd_dp_rsp_bind_by_cfg(gd_dp_rsp_t dp_rsp, cfg_t cfg_respons, error_monitor_t em);

int gd_dp_rsp_unbind_numeric(gd_dp_rsp_t dp, int32_t cmd);
int gd_dp_rsp_unbind_string(gd_dp_rsp_t dp, const char * cmd);

/*rsp find operations*/
gd_dp_rsp_t gd_dp_rsp_find_by_name(gd_dp_mgr_t dp, const char * name);
gd_dp_rsp_t gd_dp_rsp_find_first_by_numeric(gd_dp_mgr_t dp, int32_t cmd);
gd_dp_rsp_t gd_dp_rsp_find_first_by_string(gd_dp_mgr_t dp, const char * cmd);
void gd_dp_rsp_find_by_numeric(gd_dp_rsp_it_t it, gd_dp_mgr_t dp, int32_t cmd);
void gd_dp_rsp_find_by_string(gd_dp_rsp_it_t it, gd_dp_mgr_t dp, const char * cmd);

int gd_dp_dispatch_by_string(cpe_hash_string_t cmd, gd_dp_req_t req, error_monitor_t em);
int gd_dp_dispatch_by_numeric(int32_t cmd, gd_dp_req_t req, error_monitor_t em);
int gd_dp_dispatch_by_name(const char * name, gd_dp_req_t req, error_monitor_t em);

/*iterator operations*/
#define gd_dp_rsp_next(it) (it)->m_next_fun((it))

#ifdef __cplusplus
}
#endif

#endif
