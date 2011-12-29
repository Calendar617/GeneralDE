#ifndef GD_OM_MAMAGE_H
#define GD_OM_MAMAGE_H
#include "gd/om/om_types.h"
#include "om_class_i.h"
#include "om_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_mgr {
    mem_allocrator_t m_alloc;
    struct gd_om_class_mgr m_classMgr;
    struct gd_om_buffer_mgr m_bufMgr;
};

#ifdef __cplusplus
}
#endif

#endif


