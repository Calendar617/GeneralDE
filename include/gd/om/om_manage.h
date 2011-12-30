#ifndef GD_OM_MANAGE_H
#define GD_OM_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_om_mgr_t
gd_om_mgr_create(
    mem_allocrator_t alloc,
    size_t page_size,
    size_t buffer_size);

void gd_om_mgr_free(gd_om_mgr_t mgr);

int gd_om_mgr_set_backend(
    gd_om_mgr_t mgr,
    gd_om_backend_t backend,
    void * backend_ctx);

gd_om_class_id_t
gd_om_mgr_add_class(
    gd_om_mgr_t omm,
    const char * className,
    size_t object_size,
    size_t align,
    error_monitor_t em);

int gd_om_mgr_add_class_with_id(
    gd_om_mgr_t omm,
    gd_om_class_id_t classId,
    const char * className,
    size_t object_size,
    size_t align,
    error_monitor_t em);

size_t gd_om_mgr_page_size(gd_om_mgr_t omm);
size_t gd_om_mgr_buf_size(gd_om_mgr_t omm);

void gd_om_mgr_buffers(struct gd_om_buffer_it * it, gd_om_mgr_t omm);
void * gd_om_next_buffer(struct gd_om_buffer_it * it);

void gd_om_mgr_buffer_ids(struct gd_om_buffer_id_it * it, gd_om_mgr_t omm);
gd_om_buffer_id_t gd_om_next_buffer_id(struct gd_om_buffer_id_it * it);

int gd_om_mgr_set_backend_memory(gd_om_mgr_t omm, mem_allocrator_t alloc);

#ifdef __cplusplus
}
#endif

#endif
