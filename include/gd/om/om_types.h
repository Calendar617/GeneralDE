#ifndef GD_OM_TYPES_H
#define GD_OM_TYPES_H
#include "cpe/pal/types.h"
#include "cpe/utils/hash_string.h"

#define GD_OM_MAX_TYPENAME_LEN 32
#define GD_OM_MAX_TYPE_COUNT 255 /*typeid 0 is invalid!*/

#define GD_OM_INVALID_CLASSID ((gd_om_class_id_t)0)
#define GD_OM_INVALID_OID ((gd_om_oid_t)0)

typedef uint8_t gd_om_class_id_t;
typedef uint32_t gd_om_oid_t;

typedef struct gd_om_mgr * gd_om_mgr_t;
typedef struct gd_om_type * gd_om_type_t;

#endif
