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

#define gd_om_data_page_head_is_valid(p) ((p)->m_classId != GD_OM_INVALID_CLASSID)

#define gd_om_data_page_head_init(p) do {       \
    struct gd_om_data_page_head * __p =         \
        (struct gd_om_data_page_head *)(p);     \
    __p->m_magic = GD_OM_PAGE_MAGIC;            \
    __p->m_classId = GD_OM_INVALID_CLASSID;     \
    __p->m_reserve = 0;                         \
    __p->m_reserve2 = 0;                        \
    __p->m_page_idx = -1;                       \
} while(0)

#ifdef __cplusplus
}
#endif

#endif
