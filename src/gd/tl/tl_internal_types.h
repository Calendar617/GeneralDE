#ifndef GD_TL_INTERNAL_TYPES_H
#define GD_TL_INTERNAL_TYPES_H
#include "gd/tl/tl_types.h"

#define GD_TL_TL_MAX 10

struct gd_tl {
    gd_tl_manage_t m_manage;
};

struct gd_tl_manage {
    mem_allocrator_t m_alloc;

    int m_tl_count;
    struct gd_tl m_tls[GD_TL_TL_MAX];

    
};

#endif


