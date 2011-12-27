#ifndef GD_OM_CLASS_H
#define GD_OM_CLASS_H
#include "cpe/utils/hash.h"
#include "gd/om/om_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gd_om_class {
    gd_om_class_id_t m_id;
    char m_name_buf[cpe_hs_len_to_binary_len(GD_OM_MAX_TYPENAME_LEN)];
    cpe_hash_string_t m_name;
    struct cpe_hash_entry m_hh;
};

struct gd_om_class_mgr {
    struct gd_om_class m_classes[GD_OM_MAX_TYPE_COUNT + 1];
    struct cpe_hash_table m_classNameIdx;
};

int gd_om_class_mgr_init(struct gd_om_class_mgr * classMgr, mem_allocrator_t alloc);
void gd_om_class_mgr_fini(struct gd_om_class_mgr * classMgr);

struct gd_om_class *
gd_om_class_get(struct gd_om_class_mgr * classMgr, gd_om_class_id_t classId);

struct gd_om_class *
gd_om_class_find(struct gd_om_class_mgr * classMgr, cpe_hash_string_t className);

#ifdef __cplusplus
}
#endif

#endif


