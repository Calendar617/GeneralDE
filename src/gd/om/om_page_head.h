#ifndef GD_OM_PAGEHEAD_H
#define GD_OM_PAGEHEAD_H
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

#define GD_OM_PAGE_MAGIC ((uint16_t)0x16BC04FE)

struct gd_om_data_page_head {
    uint16_t m_magic;
    gd_om_class_id_t m_classId;
    int8_t m_reserve;
    int16_t m_page_idx;
    int16_t m_reserve2;
};

#pragma pack(pop)

#define gd_om_data_page_is_valid(p) ((p)->m_classId != GD_OM_INVALID_CLASSID)

#ifdef __cplusplus
}
#endif

#endif
