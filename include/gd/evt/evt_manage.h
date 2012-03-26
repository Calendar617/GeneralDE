#ifndef GD_DP_EVT_MANAGE_H
#define GD_DP_EVT_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/tl/tl_types.h"
#include "evt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_evt_mgr_t
gd_evt_mgr_create(
    tl_t tl,
    LPDRMETALIB metalib,
    mem_allocrator_t alloc);

void gd_evt_mgr_free(gd_evt_mgr_t em);

LPDRMETALIB
gd_evt_mgr_metalib(gd_evt_mgr_t em);

#ifdef __cplusplus
}
#endif

#endif
