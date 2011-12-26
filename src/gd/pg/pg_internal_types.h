#ifndef GD_PG_INTERNAL_TYPES_H
#define GD_PG_INTERNAL_TYPES_H
#include "cpe/pal/queue.h"
#include "cpe/utils/memory.h"
#include "gd/pg/pg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_pg_buf {
};

struct gd_pg_info {
    char m_page_type_name[16];
    size_t m_entry_size;
};

struct gd_pg_mgr_control_page {
};

struct gd_pg_mgr {
    mem_allocrator_t m_alloc;
    gd_pg_backend_t m_backend;
    size_t m_pageSize;
};

#ifdef __cplusplus
}
#endif

#endif
