#ifndef GD_OM_PAGE_TYPES_H
#define GD_OM_PAGE_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/memory.h"
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_buf {
};

#pragma pack(push, 1)
struct gd_om_alloc_info {
    uint8_t m_pageType;
    uint8_t m_state;
};

struct gd_om_info {
    char m_page_type_name[GD_OM_MAX_TYPENAME_LEN];
    size_t m_entry_size;
};

#pragma pack(pop)

struct gd_om_mgr_control_page {
};

#ifdef __cplusplus
}
#endif

#endif
