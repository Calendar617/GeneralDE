#include <assert.h>
#include <strings.h>
#include "gd/cfg/cfg_manage.h"
#include "gd/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

gd_cfg_t gd_cfg_create(mem_allocrator_t alloc) {
    gd_cfg_manage_t cm;
    size_t allocSize = sizeof(struct gd_cfg_manage);

    cm = (gd_cfg_manage_t)mem_alloc(alloc, allocSize);
    bzero(cm, allocSize);

    cm->m_alloc = alloc;
    cm->m_root.m_type = GD_CFG_TYPE_STRUCT;
    cm->m_root.m_manage = cm;
    cm->m_root.m_parent = NULL;

    gd_cfg_struct_init(&cm->m_root);

    return (gd_cfg_t)&cm->m_root;
}

void gd_cfg_free(gd_cfg_t cfg) {
    if (cfg == NULL) return;

    assert(cfg->m_manage);

    if ((gd_cfg_t)&cfg->m_manage->m_root == cfg) {
        gd_cfg_struct_fini(&cfg->m_manage->m_root);
        mem_free(
            cfg->m_manage->m_alloc,
            ((char*)cfg) - (sizeof(struct gd_cfg_manage) - sizeof(struct gd_cfg_struct)));
    }
    else {
        assert(cfg->m_parent);

        if (cfg->m_parent->m_type == GD_CFG_TYPE_STRUCT) {
            gd_cfg_struct_item_delete((struct gd_cfg_struct *)cfg->m_parent, cfg);
        }
        else {
            assert(cfg->m_parent->m_type == GD_CFG_TYPE_SEQUENCE);
            gd_cfg_seq_item_delete((struct gd_cfg_seq *)cfg->m_parent, cfg);
        }
    }
}

void gd_cfg_fini(gd_cfg_t cfg) {
    switch (cfg->m_type) {
    case GD_CFG_TYPE_STRUCT:
        gd_cfg_struct_fini((struct gd_cfg_struct *)(cfg));
        break;
    case GD_CFG_TYPE_SEQUENCE:
        gd_cfg_seq_fini((struct gd_cfg_seq *)(cfg));
        break;
    default:
        //do nothing for other
        break;
    }
}

gd_cfg_t gd_cfg_struct_add_struct(gd_cfg_t s, const char * name) {
    gd_cfg_t rv = gd_cfg_struct_item_create((struct gd_cfg_struct *)s, name, GD_CFG_TYPE_STRUCT, sizeof(struct gd_cfg_struct));
    if (rv == NULL) return NULL;
    gd_cfg_struct_init((struct gd_cfg_struct *)rv);
    return rv;
}

gd_cfg_t gd_cfg_struct_add_seq(gd_cfg_t s, const char * name) {
    gd_cfg_t rv = gd_cfg_struct_item_create((struct gd_cfg_struct *)s, name, GD_CFG_TYPE_SEQUENCE, sizeof(struct gd_cfg_seq));
    if (rv == NULL) return NULL;
    gd_cfg_seq_init((struct gd_cfg_seq *)rv);
    return rv;
}

#define GD_CFG_GEN_STRUCT_ADD_TYPE(__type, __typeId)                    \
gd_cfg_t gd_cfg_struct_add_ ## __type(                                  \
        gd_cfg_t s, const char * name, __type ## _t v) {                \
    gd_cfg_t rv = gd_cfg_struct_item_create(                            \
        (struct gd_cfg_struct *)s, name, __typeId, sizeof(v));          \
    if (rv == NULL) return NULL;                                        \
    *(( __type ## _t*)gd_cfg_data(rv)) = v;                             \
    return rv;                                                          \
}

GD_CFG_GEN_STRUCT_ADD_TYPE(int8, GD_CFG_TYPE_INT8)
GD_CFG_GEN_STRUCT_ADD_TYPE(uint8, GD_CFG_TYPE_UINT8)
GD_CFG_GEN_STRUCT_ADD_TYPE(int16, GD_CFG_TYPE_INT16)
GD_CFG_GEN_STRUCT_ADD_TYPE(uint16, GD_CFG_TYPE_UINT16)
GD_CFG_GEN_STRUCT_ADD_TYPE(int32, GD_CFG_TYPE_INT32)
GD_CFG_GEN_STRUCT_ADD_TYPE(uint32, GD_CFG_TYPE_UINT32)
GD_CFG_GEN_STRUCT_ADD_TYPE(int64, GD_CFG_TYPE_INT64)
GD_CFG_GEN_STRUCT_ADD_TYPE(uint64, GD_CFG_TYPE_UINT64)
