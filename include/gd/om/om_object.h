#ifndef GD_OM_OBJECT_H
#define GD_OM_OBJECT_H
#include "cpe/utils/memory.h"
#include "cpe/utils/error.h"
#include "om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_om_oid_t
gd_om_obj_alloc(
    gd_om_mgr_t omm,
    cpe_hash_string_t className,
    error_monitor_t em);

void gd_om_obj_free(
    gd_om_mgr_t omm,
    gd_om_oid_t oid,
    error_monitor_t em);

void * gd_om_obj_get(
    gd_om_mgr_t omm, 
    gd_om_oid_t oid,
    error_monitor_t em);

gd_om_oid_t
gd_om_obj_id_from_addr(
    gd_om_mgr_t omm,
    void * data,
    error_monitor_t em);

gd_om_class_t
gd_om_obj_class(
    gd_om_mgr_t omm,
    gd_om_oid_t oid,
    error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
