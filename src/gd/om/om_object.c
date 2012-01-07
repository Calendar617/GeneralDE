#include <assert.h>
#include "gd/om/om_object.h"
#include "gd/om/om_error.h"
#include "om_manage_i.h"
#include "om_page_head.h"

gd_om_oid_t gd_om_obj_alloc(
    gd_om_mgr_t omm,
    cpe_hash_string_t className,
    error_monitor_t em)
{
    struct gd_om_class * theClass;
    int32_t baseOid;

    assert(omm);
    assert(className);

    theClass = gd_om_class_find(&omm->m_classMgr, className);
    if (theClass == NULL) {
        CPE_ERROR_EX(
            em, gd_om_class_not_exist,
            "theClass %s not exist!", cpe_hs_data(className));
        return GD_OM_INVALID_OID; 
    }

    baseOid = gd_om_class_alloc_object(theClass);
    if (baseOid < 0) {
        void * newPage = gd_om_page_get(&omm->m_bufMgr, em);
        if (newPage == NULL) {
            CPE_ERROR_EX(
                em, gd_om_no_memory,
                "object of theClass %s alloc new page fail!", cpe_hs_data(className));
            return GD_OM_INVALID_OID;
        }

        if (gd_om_class_add_page(theClass, newPage, em) != 0) {
            return GD_OM_INVALID_OID;
        }

        baseOid = gd_om_class_alloc_object(theClass);
        if (baseOid < 0) {
            CPE_ERROR_EX(
                em, gd_om_no_memory,
                "object of theClass %s alloc oid fail!", cpe_hs_data(className));
            return GD_OM_INVALID_OID;
        }
    }

    if (baseOid > 0xFFFFFF) {
        CPE_ERROR_EX(
            em, gd_om_no_memory,
            "object of theClass %s count overflow!", cpe_hs_data(className));
        gd_om_class_free_object(theClass, baseOid, NULL);
        return GD_OM_INVALID_OID; 
    }

    return (((uint32_t)theClass->m_id) << 24) | ((uint32_t)baseOid);
}

void gd_om_obj_free(
    gd_om_mgr_t omm,
    gd_om_oid_t oid,
    error_monitor_t em)
{
    gd_om_class_id_t classId;
    struct gd_om_class * theClass;

    assert(omm);

    classId = ((uint32_t)oid) >> 24;
    theClass = gd_om_class_get(&omm->m_classMgr, classId);

    if (theClass == NULL) {
        CPE_ERROR_EX(em, gd_om_class_not_exist, "theClass id=%d not exist!", classId);
        return;
    }

    gd_om_class_free_object(theClass, oid & 0xFFFFFF, em);
}

void * gd_om_obj_get(
    gd_om_mgr_t omm, 
    gd_om_oid_t oid,
    error_monitor_t em)
{
    gd_om_class_id_t classId;
    struct gd_om_class * theClass;

    assert(omm);

    classId = ((uint32_t)oid) >> 24;
    theClass = gd_om_class_get(&omm->m_classMgr, classId);

    if (theClass == NULL) {
        CPE_ERROR_EX(
            em, gd_om_class_not_exist,
            "theClass id=%d not exist!", classId);
        return NULL;
    }

    return gd_om_class_get_object(theClass, oid & 0xFFFFFF, em);
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

gd_om_oid_t
gd_om_obj_id_from_addr(
    gd_om_mgr_t omm,
    void * data,
    error_monitor_t em)
{
    void * page;
    struct gd_om_data_page_head * head;
    struct gd_om_class * theClass;
    int32_t baseOid;

    assert(omm);

    page = gd_om_buffer_mgr_find_page(&omm->m_bufMgr, data);
    if (page == NULL) {
        CPE_ERROR_EX(
            em, gd_om_invalid_address,
            "address to oid: address is invalid");
        return GD_OM_INVALID_OID;
    }

    head = (struct gd_om_data_page_head *)page;

    if (head->m_classId == GD_OM_INVALID_CLASSID) {
        CPE_ERROR_EX(
            em, gd_om_invalid_address,
            "address to oid: address is invalid, not allocked!");
        return GD_OM_INVALID_OID;
    }

    theClass = gd_om_class_get(&omm->m_classMgr, head->m_classId);
    if (theClass == NULL) {
        CPE_ERROR_EX(
            em, gd_om_class_not_exist,
            "theClass id=%d not exist!", head->m_classId);
        return GD_OM_INVALID_OID;
    }

    baseOid = gd_om_class_addr_2_object(theClass, page, data);
    if (baseOid < 0) {
        CPE_ERROR_EX(
            em, gd_om_invalid_address,
            "address to oid: internal error, get base oid fail!");
        return GD_OM_INVALID_OID;
    }

    return (((uint32_t)theClass->m_id) << 24) | ((uint32_t)baseOid);
}
