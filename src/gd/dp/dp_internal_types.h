#ifndef GD_DP_IMPL_INTERNAL_TYPES_H
#define GD_DP_IMPL_INTERNAL_TYPES_H
#include "cpe/utils/memory.h"
#include "gd/dp/dp_types.h"

struct gd_dp_mgr {
    mem_allocrator_t m_alloc;
    gd_tl_t m_timerTl;
};

struct gd_dp_rsp {
    gd_dp_mgr_t m_mgr;
};

struct gd_dp_req {
    gd_dp_mgr_t m_mgr;
    mem_allocrator_t m_talloc;

    void * m_data;
    size_t m_data_size;
};

#endif
