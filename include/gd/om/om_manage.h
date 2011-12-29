#ifndef GD_OM_MANAGE_H
#define GD_OM_MANAGE_H
#include "cpe/utils/memory.h"
#include "om_types.h"

gd_om_mgr_t
gd_om_mgr_create(
    mem_allocrator_t alloc,
    size_t page_size,
    size_t buffer_size,
    gd_om_backend_t backend,
    void * backend_ctx);

void gd_om_mgr_free(gd_om_mgr_t mgr);

typedef enum gd_om_mgr_opt {
    gd_om_mgr_set_saver
} gd_om_mgr_opt_t;

int gd_om_mgr_set_opt(gd_om_mgr_opt_t opt, ...);

#endif


