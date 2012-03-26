#ifndef GD_EVT_INTERNAL_H
#define GD_EVT_INTERNAL_H
#include "cpe/tl/tl_types.h"
#include "gd/evt/evt_types.h"

struct gd_evt_mgr {
    mem_allocrator_t m_alloc;
    LPDRMETALIB m_metalib;
    tl_t m_tl;
};

struct gd_evt {
    LPDRMETA m_meta;
    size_t m_data_capacity;
};

#endif

