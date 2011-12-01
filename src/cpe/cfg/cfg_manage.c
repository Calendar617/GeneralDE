#include <assert.h>
#include <string.h>
#include <strings.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

cfg_t cfg_create(mem_allocrator_t alloc) {
    cfg_manage_t cm;
    size_t allocSize = sizeof(struct cfg_manage);

    cm = (cfg_manage_t)mem_alloc(alloc, allocSize);
    bzero(cm, allocSize);

    cm->m_alloc = alloc;
    cm->m_root.m_type = CPE_CFG_TYPE_STRUCT;
    cm->m_root.m_manage = cm;
    cm->m_root.m_parent = NULL;

    cfg_struct_init(&cm->m_root);

    return (cfg_t)&cm->m_root;
}

void cfg_free(cfg_t cfg) {
    if (cfg == NULL) return;

    assert(cfg->m_manage);

    if ((cfg_t)&cfg->m_manage->m_root == cfg) {
        cfg_struct_fini(&cfg->m_manage->m_root);
        mem_free(
            cfg->m_manage->m_alloc,
            ((char*)cfg) - (sizeof(struct cfg_manage) - sizeof(struct cfg_struct)));
    }
    else {
        assert(cfg->m_parent);

        if (cfg->m_parent->m_type == CPE_CFG_TYPE_STRUCT) {
            cfg_struct_item_delete((struct cfg_struct *)cfg->m_parent, cfg);
        }
        else {
            assert(cfg->m_parent->m_type == CPE_CFG_TYPE_SEQUENCE);
            cfg_seq_item_delete((struct cfg_seq *)cfg->m_parent, cfg);
        }
    }
}

void cfg_fini(cfg_t cfg) {
    switch (cfg->m_type) {
    case CPE_CFG_TYPE_STRUCT:
        cfg_struct_fini((struct cfg_struct *)(cfg));
        break;
    case CPE_CFG_TYPE_SEQUENCE:
        cfg_seq_fini((struct cfg_seq *)(cfg));
        break;
    default:
        //do nothing for other
        break;
    }
}

cfg_t cfg_struct_add_struct(cfg_t s, const char * name) {
    cfg_t rv = cfg_struct_item_create(
        (struct cfg_struct *)s,
        name,
        CPE_CFG_TYPE_STRUCT,
        sizeof(struct cfg_struct) - sizeof(struct cfg));
    if (rv == NULL) return NULL;
    cfg_struct_init((struct cfg_struct *)rv);
    return rv;
}

cfg_t cfg_seq_add_struct(cfg_t s) {
    cfg_t rv = cfg_seq_item_create(
        (struct cfg_seq *)s,
        CPE_CFG_TYPE_STRUCT,
        sizeof(struct cfg_struct) - sizeof(struct cfg));
    if (rv == NULL) return NULL;
    cfg_struct_init((struct cfg_struct *)rv);
    return rv;
}

cfg_t cfg_struct_add_seq(cfg_t s, const char * name) {
    cfg_t rv = cfg_struct_item_create(
        (struct cfg_struct *)s,
        name,
        CPE_CFG_TYPE_SEQUENCE,
        sizeof(struct cfg_seq) - sizeof(struct cfg));
    if (rv == NULL) return NULL;
    cfg_seq_init((struct cfg_seq *)rv);
    return rv;
}

cfg_t cfg_seq_add_seq(cfg_t s) {
    cfg_t rv = cfg_seq_item_create(
        (struct cfg_seq *)s,
        CPE_CFG_TYPE_SEQUENCE,
        sizeof(struct cfg_seq) - sizeof(struct cfg));
    if (rv == NULL) return NULL;
    cfg_seq_init((struct cfg_seq *)rv);
    return rv;
}

cfg_t cfg_struct_add_string(cfg_t s, const char * name, const char * value) {
    size_t len = strlen(value) + 1;
    cfg_t rv = cfg_struct_item_create(
        (struct cfg_struct *)s, name, CPE_CFG_TYPE_STRING, len);
    if (rv == NULL) return NULL;
    memcpy(cfg_data(rv), value, len);
    return rv;
}

cfg_t cfg_seq_add_string(cfg_t s, const char * value) {
    size_t len = strlen(value) + 1;
    cfg_t rv = cfg_seq_item_create((struct cfg_seq *)s, CPE_CFG_TYPE_STRING, len);
    if (rv == NULL) return NULL;
    memcpy(cfg_data(rv), value, len);
    return rv;
}

#define CPE_CFG_GEN_ADD_FUN_TYPE(__type, __typeId)                       \
cfg_t cfg_struct_add_ ## __type(                                  \
        cfg_t s, const char * name, __type ## _t v) {                \
    cfg_t rv = cfg_struct_item_create(                            \
        (struct cfg_struct *)s, name, __typeId, sizeof(v));          \
    if (rv == NULL) return NULL;                                        \
    *(( __type ## _t*)cfg_data(rv)) = v;                             \
    return rv;                                                          \
}                                                                       \
cfg_t cfg_seq_add_ ## __type(cfg_t s, __type ## _t v) {        \
    cfg_t rv = cfg_seq_item_create(                               \
        (struct cfg_seq *)s, __typeId, sizeof(v));                   \
    if (rv == NULL) return NULL;                                        \
    *(( __type ## _t*)cfg_data(rv)) = v;                             \
    return rv;                                                          \
}

CPE_CFG_GEN_ADD_FUN_TYPE(int8, CPE_CFG_TYPE_INT8)
CPE_CFG_GEN_ADD_FUN_TYPE(uint8, CPE_CFG_TYPE_UINT8)
CPE_CFG_GEN_ADD_FUN_TYPE(int16, CPE_CFG_TYPE_INT16)
CPE_CFG_GEN_ADD_FUN_TYPE(uint16, CPE_CFG_TYPE_UINT16)
CPE_CFG_GEN_ADD_FUN_TYPE(int32, CPE_CFG_TYPE_INT32)
CPE_CFG_GEN_ADD_FUN_TYPE(uint32, CPE_CFG_TYPE_UINT32)
CPE_CFG_GEN_ADD_FUN_TYPE(int64, CPE_CFG_TYPE_INT64)
CPE_CFG_GEN_ADD_FUN_TYPE(uint64, CPE_CFG_TYPE_UINT64)

