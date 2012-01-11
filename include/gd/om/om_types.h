#ifndef GD_OM_TYPES_H
#define GD_OM_TYPES_H
#include "cpe/pal/pal_types.h"
#include "cpe/utils/hash_string.h"
#include "cpe/utils/range.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GD_OM_MAX_TYPENAME_LEN 32
#define GD_OM_MAX_TYPE_COUNT 255 /*typeid 0 is invalid!*/

#define GD_OM_INVALID_CLASSID ((gd_om_class_id_t)0)
#define GD_OM_INVALID_OID ((gd_om_oid_t)0)
#define GD_OM_INVALID_BUFFER_ID ((gd_om_oid_t)0)

typedef uint8_t gd_om_class_id_t;
typedef uint32_t gd_om_oid_t;

typedef struct gd_om_backend * gd_om_backend_t;
typedef struct gd_om_class * gd_om_class_t;
typedef struct gd_om_mgr * gd_om_mgr_t;
typedef struct gd_om_type * gd_om_type_t;

typedef ptr_int_t gd_om_buffer_id_t;

struct gd_om_buffer_it {
    size_t m_buf_size;
    struct cpe_range_it m_range_it;
    struct cpe_range m_curent;
};

struct gd_om_buffer_id_it {
    struct cpe_range_it m_range_it;
    struct cpe_range m_curent;
};

struct gd_om_backend {
    gd_om_buffer_id_t (*buf_alloc)(size_t size, void * context);
    void * (*buf_get)(gd_om_buffer_id_t id, void * context);
    void (*clear)(struct gd_om_buffer_id_it * buf_ids, void * context);
};

#ifdef __cplusplus
}
#endif

#endif
