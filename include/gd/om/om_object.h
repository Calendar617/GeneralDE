#ifndef GD_OM_OBJECT_H
#define GD_OM_OBJECT_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "om_types.h"

gd_om_oid_t gd_om_obj_alloc(
    gd_om_mgr_t omm,
    cpe_hash_string_t className,
    error_monitor_t em);

void gd_om_obj_free(gd_om_mgr_t omm, gd_om_oid_t);

void * gd_om_obj_get(gd_om_mgr_t omm, gd_om_oid_t oid);
cpe_hash_string_t gd_om_obj_type(gd_om_mgr_t omm, gd_om_oid_t oid);

#endif
