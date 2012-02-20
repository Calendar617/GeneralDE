#ifndef GD_OM_CLASS_H
#define GD_OM_CLASS_H
#include "om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_om_class_t gd_om_mgr_get_class(gd_om_mgr_t omm, gd_om_class_id_t);
gd_om_class_t gd_om_mgr_find_class(gd_om_mgr_t omm, cpe_hash_string_t className);

gd_om_class_id_t om_class_id(gd_om_class_t cls);
const char * gd_om_class_name(gd_om_class_t cls);
cpe_hash_string_t gd_om_class_name_hs(gd_om_class_t cls);
size_t gd_om_class_obj_size(gd_om_class_t cls);

#ifdef __cplusplus
}
#endif

#endif
