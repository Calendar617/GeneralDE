#ifndef GD_OM_MAMAGE_H
#define GD_OM_MAMAGE_H
#include "gd/om/om_types.h"
#include "om_class.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_mgr {
    mem_allocrator_t m_alloc;
    size_t m_pageSize;
    size_t m_pagePerFrame;
    struct gd_om_class_mgr m_classMgr;
};

#ifdef __cplusplus
}
#endif

#endif


