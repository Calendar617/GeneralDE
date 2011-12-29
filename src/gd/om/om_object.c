#include <assert.h>
#include "gd/om/om_object.h"
#include "gd/om/om_error.h"
#include "om_manage_i.h"

gd_om_oid_t gd_om_obj_alloc(
    gd_om_mgr_t omm,
    cpe_hash_string_t className,
    error_monitor_t em)
{
    struct gd_om_class * class;
    int32_t baseOid;

    assert(omm);
    assert(className);

    class = gd_om_class_find(&omm->m_classMgr, className);
    if (class == NULL) {
        CPE_ERROR_EX(
            em, gd_om_error_no_class,
            "class %s not exist!", cpe_hs_data(className));
        return GD_OM_INVALID_OID; 
    }

    baseOid = gd_om_class_alloc_object(class);
    if (baseOid < 0) {
        void * newPage = gd_om_page_get(&omm->m_bufMgr, em);
        if (newPage == NULL) {
            CPE_ERROR_EX(
                em, gd_om_no_memory,
                "object of class %s alloc new page fail!", cpe_hs_data(className));
            return GD_OM_INVALID_OID;
        }

        if (gd_om_class_add_new_page(class, newPage, em) != 0) {
            return GD_OM_INVALID_OID;
        }

        baseOid = gd_om_class_alloc_object(class);
        if (baseOid < 0) {
            CPE_ERROR_EX(
                em, gd_om_no_memory,
                "object of class %s alloc oid fail!", cpe_hs_data(className));
            return GD_OM_INVALID_OID;
        }
    }

    if (baseOid > 0xFFFFFF) {
        CPE_ERROR_EX(
            em, gd_om_no_memory,
            "object of class %s count overflow!", cpe_hs_data(className));
        gd_om_class_free_object(class, baseOid, NULL);
        return GD_OM_INVALID_OID; 
    }

    return (((uint32_t)class->m_id) << 24) | ((uint32_t)baseOid);
}

void gd_om_obj_free(
    gd_om_mgr_t omm,
    gd_om_oid_t oid,
    error_monitor_t em)
{
    gd_om_class_id_t classId;
    struct gd_om_class * class;

    assert(omm);

    classId = ((uint32_t)oid) >> 24;
    class = gd_om_class_get(&omm->m_classMgr, classId);

    if (class == NULL) {
        CPE_ERROR_EX(em, gd_om_error_no_class, "class id=%d not exist!", classId);
        return;
    }

    gd_om_class_free_object(class, oid & 0xFFFFFF, em);
}

void * gd_om_obj_get(
    gd_om_mgr_t omm, 
    gd_om_oid_t oid,
    error_monitor_t em)
{
    gd_om_class_id_t classId;
    struct gd_om_class * class;

    assert(omm);

    classId = ((uint32_t)oid) >> 24;
    class = gd_om_class_get(&omm->m_classMgr, classId);

    if (class == NULL) {
        CPE_ERROR_EX(em, gd_om_error_no_class, "class id=%d not exist!", classId);
        return NULL;
    }

    return gd_om_class_get_object(class, oid & 0xFFFFFF, em);
}

gd_om_class_t
gd_om_obj_class(
    gd_om_mgr_t omm,
    gd_om_oid_t oid,
    error_monitor_t em)
{
    gd_om_class_id_t classId;

    assert(omm);

    classId = ((uint32_t)oid) >> 24;
    return gd_om_class_get(&omm->m_classMgr, classId);
}
