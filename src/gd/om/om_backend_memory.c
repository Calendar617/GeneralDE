#include "gd/om/om_manage.h"

gd_om_buffer_id_t gd_om_backend_mem_alloc(size_t size, void * context) {
    return (gd_om_buffer_id_t)mem_alloc((mem_allocrator_t)context, size);
}

void gd_om_backend_mem_clear(struct gd_om_buffer_id_it * buf_ids, void * context) {
    gd_om_buffer_id_t bufId;
    while((bufId = gd_om_next_buffer_id(buf_ids)) != GD_OM_INVALID_BUFFER_ID) {
        mem_free((mem_allocrator_t)context, (void*)bufId);
    }
}

struct gd_om_backend g_gd_om_backend_mem = {
    gd_om_backend_mem_alloc
    , NULL
    , gd_om_backend_mem_clear
};

int gd_om_mgr_set_backend_memory(gd_om_mgr_t omm, mem_allocrator_t alloc) {
    return gd_om_mgr_set_backend(omm, &g_gd_om_backend_mem, alloc);
}
