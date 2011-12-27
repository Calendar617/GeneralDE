#ifndef GD_OM_PAGE_TYPES_H
#define GD_OM_PAGE_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/memory.h"
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

struct gd_om_page_head {
    int16_t m_magic;
    gd_om_class_id_t m_classId;
    int8_t m_reserve;
    int16_t m_page_idx;
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
