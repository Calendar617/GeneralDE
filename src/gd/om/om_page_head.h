#ifndef GD_OM_PAGE_TYPES_H
#define GD_OM_PAGE_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/memory.h"
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

#ifdef __cplusplus
}
#endif

#endif
