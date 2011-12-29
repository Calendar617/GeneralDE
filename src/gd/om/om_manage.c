#include <assert.h>
#include "gd/om/om_manage.h"
#include "om_manage_i.h"

gd_om_mgr_t
gd_om_mgr_create(
    mem_allocrator_t alloc,
    size_t page_size,
    size_t buffer_size,
    gd_om_backend_t backend,
    void * backend_ctx)
{
    gd_om_mgr_t omm = mem_alloc(alloc, sizeof(struct gd_om_mgr));
    if (omm == NULL) return NULL;

    omm->m_alloc = alloc;

    if (
        gd_om_buffer_mgr_init(
            &omm->m_bufMgr, backend, backend_ctx, page_size, buffer_size, alloc)
        != 0)
    {
        mem_free(alloc, omm);
        return NULL;
    }

    if (gd_om_class_mgr_init(&omm->m_classMgr, alloc) != 0) {
        gd_om_buffer_mgr_fini(&omm->m_bufMgr);
        mem_free(alloc, omm);
        return NULL;
    }

    return omm;
}

void gd_om_mgr_free(gd_om_mgr_t omm) {
    if (omm == NULL) return;

    gd_om_class_mgr_fini(&omm->m_classMgr);
    gd_om_buffer_mgr_fini(&omm->m_bufMgr);
    
    mem_free(omm->m_alloc, omm);
}

int gd_om_mgr_set_opt(gd_om_mgr_opt_t opt, ...) {
    return 0;
}

gd_om_class_id_t
gd_om_mgr_add_class(
    gd_om_mgr_t omm,
    const char * className,
    size_t object_size,
    size_t align,
    error_monitor_t em)
{
    assert(omm);
    assert(className);

    return gd_om_class_add(
        &omm->m_classMgr,
        className,
        object_size, 
        omm->m_bufMgr.m_page_size,
        align,
        em);
}

int gd_om_mgr_add_class_with_id(
    gd_om_mgr_t omm,
    gd_om_class_id_t classId,
    const char * className,
    size_t object_size,
    size_t align,
    error_monitor_t em)
{
    assert(omm);
    assert(className);

    return gd_om_class_add_with_id(
        &omm->m_classMgr,
        classId,
        className,
        object_size, 
        omm->m_bufMgr.m_page_size,
        align,
        em);
}

gd_om_class_t
gd_om_mgr_get_class(gd_om_mgr_t omm, gd_om_class_id_t classId) {
    assert(omm);
    return gd_om_class_get(&omm->m_classMgr, classId);
}

gd_om_class_t
gd_om_mgr_find_class(gd_om_mgr_t omm, cpe_hash_string_t className) {
    assert(omm);
    return gd_om_class_find(&omm->m_classMgr, className);
}
