#include <assert.h>
#include "gd/om/om_error.h"
#include "om_buffer.h"

int gd_om_buffer_mgr_init(
    struct gd_om_buffer_mgr * pgm,
    gd_om_backend_t backend,
    void * backend_ctx,
    size_t page_size,
    size_t buf_size,
    mem_allocrator_t alloc)
{
    assert(pgm);
    assert(backend);

    pgm->m_backend = backend;
    pgm->m_backend_ctx = backend_ctx;
    pgm->m_page_size = page_size;
    pgm->m_buf_size = buf_size;

    if (cpe_range_mgr_init(&pgm->m_free_pages, alloc) != 0) {
        return -1;
    }

    if (cpe_range_mgr_init(&pgm->m_buffers, alloc) != 0) {
        cpe_range_mgr_fini(&pgm->m_free_pages);
        return -1;
    }

    if (cpe_range_mgr_init(&pgm->m_buffer_ids, alloc) != 0) {
        cpe_range_mgr_fini(&pgm->m_free_pages);
        cpe_range_mgr_fini(&pgm->m_buffers);
        return -1;
    }

    return 0;
}

void gd_om_buffer_mgr_fini(struct gd_om_buffer_mgr * pgm) {
    assert(pgm);

    cpe_range_mgr_fini(&pgm->m_buffer_ids);
    cpe_range_mgr_fini(&pgm->m_buffers);
    cpe_range_mgr_fini(&pgm->m_free_pages);
}

static int gd_om_buffer_mgr_add_new_buffer(
    struct gd_om_buffer_mgr * pgm,
    error_monitor_t em)
{
    gd_om_buffer_id_t new_buf_id;
    void * new_buf;

    assert(pgm->m_backend);

    if (pgm->m_backend->buf_alloc == NULL) {
        CPE_ERROR_EX(em, gd_om_no_memory, "backend not support alloc new buf!");
        return -1;
    }

    if (cpe_range_mgr_reserve_for_put(&pgm->m_buffers, 1) != 0) {
        CPE_ERROR_EX(em, gd_om_no_memory, "reserve for buffers fail!");
        return -1;
    }

    if (cpe_range_mgr_reserve_for_put(&pgm->m_free_pages, 1) != 0) {
        CPE_ERROR_EX(em, gd_om_no_memory, "reserve for free pages fail!");
        return -1;
    }

    if (cpe_range_mgr_reserve_for_put(&pgm->m_buffer_ids, 1) != 0) {
        CPE_ERROR_EX(em, gd_om_no_memory, "reserve for buffer ids fail!");
        return -1;
    }

    new_buf_id = pgm->m_backend->buf_alloc(pgm->m_buf_size, pgm->m_backend_ctx);
    if (new_buf_id == GD_OM_INVALID_BUFFER_ID) {
        CPE_ERROR_EX(em, gd_om_no_memory, "backend alloc new buf fail!");
        return -1;
    }

    if (pgm->m_backend->buf_get) {
        new_buf = pgm->m_backend->buf_get(new_buf_id, pgm->m_backend_ctx);
        if (new_buf == NULL) {
            CPE_ERROR_EX(em, gd_om_no_memory, "backend get buf %d fail!", new_buf_id);
            return -1;
        }
    }
    else {
        new_buf = (void *)new_buf_id;
    }

    cpe_range_put_range(&pgm->m_buffers, (int)new_buf, ((int)new_buf) + pgm->m_buf_size);
    cpe_range_put_range(&pgm->m_free_pages, (int)new_buf, ((int)new_buf) + pgm->m_buf_size);
    cpe_range_put_one(&pgm->m_buffer_ids, new_buf_id);

    return 0;
}

void * gd_om_page_get(struct gd_om_buffer_mgr * pgm, error_monitor_t em) {
    int tryCount;
    struct cpe_range pageRange;

    assert(pgm);

    tryCount = 0;
    do {
        pageRange = cpe_range_get_range(&pgm->m_free_pages, pgm->m_page_size);

        if (!cpe_range_is_valid(pageRange)) {
            if (gd_om_buffer_mgr_add_new_buffer(pgm, em) != 0) {
                return NULL;
            }
            continue;
        }

        if (cpe_range_size(pageRange) < pgm->m_page_size) {
            continue;
        }

        assert(cpe_range_size(pageRange) == pgm->m_page_size);
        break;
    }
    while(tryCount < 500);

    if (cpe_range_is_valid(pageRange)
        && cpe_range_size(pageRange) == pgm->m_page_size)
    {
        return (void*)(pageRange.m_start);
    }
    else {
        return NULL;
    }
}
