#ifndef GD_DP_MANAGE_H
#define GD_DP_MANAGE_H
#include "cpe/utils/memory.h"
#include "gd/tl/tl_types.h"
#include "dp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_dp_mgr_t gd_dp_mgr_create(mem_allocrator_t alloc, gd_tl_t timerTl);
void gd_dp_mgr_free(gd_dp_mgr_t dp);

void gd_dp_mgr_dispatch(gd_dp_mgr_t dp, void * data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
