#include <assert.h>
#include "om_class.h"

uint32_t gd_om_class_hash_fun(struct gd_om_class * class) {
    return cpe_hs_value(class->m_name);
}

int gd_om_class_cmp_fun(struct gd_om_class * l, struct gd_om_class * r) {
    return cpe_hs_cmp(l->m_name, r->m_name) == 0;
}

int gd_om_class_mgr_init(struct gd_om_class_mgr * classMgr, mem_allocrator_t alloc) {
    int i;

    assert(classMgr);

    bzero(classMgr, sizeof(struct gd_om_class_mgr));

    for(i = 0; i < sizeof(classMgr->m_classes) / sizeof(struct gd_om_class); ++i) {
        struct gd_om_class * class = &classMgr->m_classes[i];
        class->m_name = (cpe_hash_string_t)class->m_name_buf;
        cpe_hash_entry_init(&class->m_hh);
    }

    if (cpe_hash_table_init(
            &classMgr->m_classNameIdx,
            alloc,
            (cpe_hash_fun_t)gd_om_class_hash_fun,
            (cpe_hash_cmp_t)gd_om_class_cmp_fun,
            CPE_HASH_OBJ2ENTRY(gd_om_class, m_hh),
            -1) != 0)
    {
        return -1;
    }

    return 0;
}

void gd_om_class_mgr_fini(struct gd_om_class_mgr * classMgr) {
    assert(classMgr);

    cpe_hash_table_fini(&classMgr->m_classNameIdx);
}

struct gd_om_class *
gd_om_class_get(struct gd_om_class_mgr * classMgr, gd_om_class_id_t classId) {
    struct gd_om_class * r;

    assert(classMgr);

    r = &classMgr->m_classes[classId];

    return r->m_id == GD_OM_INVALID_CLASSID
        ? NULL
        : r;
}

struct gd_om_class *
gd_om_class_find(struct gd_om_class_mgr * classMgr, cpe_hash_string_t className) {
    struct gd_om_class key;

    assert(classMgr);
    assert(className);

    key.m_name = className;

    return (struct gd_om_class *)cpe_hash_table_find(&classMgr->m_classNameIdx, &key);
}
