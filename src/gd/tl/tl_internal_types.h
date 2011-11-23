#ifndef GD_TL_INTERNAL_TYPES_H
#define GD_TL_INTERNAL_TYPES_H
#include "gd/tl/tl_types.h"

struct gd_tl_manage {
    mem_allocrator_t m_alloc;
};

struct gd_tl {
    gd_tl_manage_t m_manage;
};

#endif


