#include <assert.h>
#include "gd/om/om_object.h"
#include "gd/om/om_error.h"
#include "om_manage_internal.h"

gd_om_oid_t gd_om_obj_alloc(
    gd_om_mgr_t omm,
    cpe_hash_string_t className,
    error_monitor_t em)
{
    struct gd_om_class * class;

    assert(omm);
    assert(className);

    class = gd_om_class_find(&omm->m_classMgr, className);
    if (class == NULL) {
        CPE_ERROR_EX(
            em, gd_om_error_no_class,
            "class %s not exist!", cpe_hs_data(className));
        return GD_OM_INVALID_OID; 
    }
    
    return GD_OM_INVALID_OID;
}

void gd_om_obj_free(gd_om_mgr_t omm, gd_om_oid_t oid) {
    assert(omm);

    if (oid == GD_OM_INVALID_OID) return;
}
