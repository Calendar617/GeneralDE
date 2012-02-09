#ifndef GD_OM_PAGE_H
#define GD_OM_PAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "cpe/utils/range.h"
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_buffer_mgr {
    gd_om_backend_t m_backend;
    void * m_backend_ctx;
    size_t m_page_size;
    size_t m_buf_size;
    struct cpe_range_mgr m_free_pages;
    struct cpe_range_mgr m_buffers;
    struct cpe_range_mgr m_buffer_ids;
};

int gd_om_buffer_mgr_init(
    struct gd_om_buffer_mgr * pgm,
    size_t page_size,
    size_t buf_size,
    mem_allocrator_t alloc);

int gd_om_buffer_mgr_set_backend(
    struct gd_om_buffer_mgr * pgm,
    gd_om_backend_t backend,
    void * backend_ctx);

int gd_om_buffer_mgr_add_new_buffer(
    struct gd_om_buffer_mgr * pgm,
    gd_om_buffer_id_t buf_id,
    error_monitor_t em);

int gd_om_buffer_mgr_attach_old_buffer(
    struct gd_om_buffer_mgr * pgm,
    gd_om_buffer_id_t buf_id,
    error_monitor_t em);

void * gd_om_buffer_mgr_find_page(
    struct gd_om_buffer_mgr * pgm,
    void * address);

void gd_om_buffer_mgr_fini(struct gd_om_buffer_mgr * pgm);

void * gd_om_page_get(struct gd_om_buffer_mgr * pgm, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif


