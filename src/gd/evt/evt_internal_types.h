#ifndef GD_EVT_INTERNAL_H
#define GD_EVT_INTERNAL_H
#include "gd/tl/tl_types.h"
#include "gd/evt/evt_types.h"

struct gd_evt_mgr {
    mem_allocrator_t m_alloc;
    LPDRMETALIB m_metalib;
    gd_tl_t m_tl;
};

struct gd_evt {
    LPDRMETA m_meta;
};

#endif

