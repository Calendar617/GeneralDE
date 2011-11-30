#ifndef GD_DP_IMPL_INTERNAL_TYPES_H
#define GD_DP_IMPL_INTERNAL_TYPES_H
#include "cpe/utils/memory.h"
#include "gd/dp/dp_types.h"

struct gd_dp_manage {
    mem_allocrator_t m_alloc;
    gd_tl_t m_timerTl;
};

struct gd_dp_context {
    gd_dp_manage_t m_manage;
};

#endif
