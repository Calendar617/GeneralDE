#include <string.h>
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

void * cfg_data(cfg_t cfg) {
    if (cfg == NULL || !cfg_is_value(cfg)) {
        return NULL;
    }

    return (void*)(cfg + 1);
}

const char * cfg_name(cfg_t cfg) {
    if (cfg == NULL || cfg->m_parent == NULL) return "";

    switch (cfg->m_parent->m_type) {
    case CPE_CFG_TYPE_STRUCT:
        return cfg_to_struct_item(cfg)->m_name;
    case CPE_CFG_TYPE_SEQUENCE:
        return cfg_name(cfg->m_parent);
    default:
        return "";
    }
}

int cfg_type(cfg_t cfg) {
    if (cfg == NULL) return CPE_DR_TYPE_UNKOWN;

    return cfg->m_type;
}

cfg_t cfg_parent(cfg_t cfg) {
    if (cfg == NULL) return NULL;

    return cfg->m_parent;
}

int cfg_type_is_value(int type) {
    return type > CPE_DR_TYPE_COMPOSITE;
}

int cfg_is_value(cfg_t cfg) {
    return cfg->m_type > CPE_DR_TYPE_COMPOSITE;
}

#define CPE_CFG_GEN_READ_TYPE(__type)                            \
__type ## _t cfg_as_  ## __type(cfg_t at, __type ## _t dft) {    \
    __type ## _t rv;                                             \
    if (at == NULL || !cfg_is_value(at) ) return dft;            \
    return dr_ctype_try_read_ ## __type(                         \
        &rv, cfg_data(at), at->m_type, NULL) == 0                \
        ? rv                                                     \
        : dft;                                                   \
}                                                                \
__type ## _t cfg_get_  ## __type(                                \
    cfg_t cfg, const char * path, __type ## _t dft) {            \
    __type ## _t rv;                                             \
    cfg_t at = cfg_find_cfg(cfg, path);                          \
    if (at == NULL || !cfg_is_value(at) ) return dft;            \
    return dr_ctype_try_read_ ## __type(                         \
        &rv, cfg_data(at), at->m_type, NULL) == 0                \
        ? rv                                                     \
        : dft;                                                   \
}

CPE_CFG_GEN_READ_TYPE(int8);
CPE_CFG_GEN_READ_TYPE(uint8);
CPE_CFG_GEN_READ_TYPE(int16);
CPE_CFG_GEN_READ_TYPE(uint16);
CPE_CFG_GEN_READ_TYPE(int32);
CPE_CFG_GEN_READ_TYPE(uint32);
CPE_CFG_GEN_READ_TYPE(int64);
CPE_CFG_GEN_READ_TYPE(uint64);

const char * cfg_get_string(cfg_t cfg, const char * path, const char * dft) {
    cfg_t at = cfg_find_cfg(cfg, path);
    if (at == NULL) return dft;

    return at->m_type == CPE_DR_TYPE_STRING
        ? (const char *)cfg_data(at)
        : dft;
}

static cfg_t cfg_do_find_cfg_from_struct(cfg_t cfg, const char * path, const char * end) {
    char buf[CPE_CFG_NAME_MAX_LEN + 1];

    int size = end - path;
    if (size > CPE_CFG_NAME_MAX_LEN) return NULL;

    memcpy(buf, path, size);
    buf[size] = 0;
    return cfg_struct_find_cfg(cfg, buf);
}

static cfg_t cfg_do_find_cfg_from_seq(cfg_t cfg, const char * path, const char * end) {
    char buf[20 + 1];
    uint32_t seqPos;
    int size = end - path;
    if (size > 20) return NULL;

    memcpy(buf, path, size);
    buf[size] = 0;

    if (dr_ctype_set_from_string(&seqPos, CPE_DR_TYPE_UINT32, buf, NULL) != 0) return NULL;
    return cfg_seq_at(cfg, seqPos);
}

cfg_t cfg_find_cfg(cfg_t cfg, const char * path) {
    const char * end = path + strlen(path);
    const char * nextSeqTag = strchr(path, '[');
    const char * nextNameTag = strchr(path, '.');

    if (nextSeqTag == NULL) nextSeqTag = end;
    if (nextNameTag == NULL) nextNameTag = end;

    while(cfg && path < end) {
        if (path[0] == '[') {
            if (cfg->m_type != CPE_CFG_TYPE_SEQUENCE) {
                if (cfg->m_type == CPE_CFG_TYPE_STRUCT) {
                    cfg = cfg_struct_find_cfg(cfg, "");
                    if (cfg && cfg->m_type == CPE_CFG_TYPE_SEQUENCE) {
                        continue;
                    }
                }

                return NULL;
            }

            const char * seqEndTag = strchr(nextSeqTag, ']');
            if (seqEndTag == NULL) return NULL;

            cfg = cfg_do_find_cfg_from_seq(cfg, path + 1, seqEndTag);
            path = seqEndTag + 1;
            nextSeqTag = strchr(path, '[');
            if (nextSeqTag == NULL) nextSeqTag = end;

            if (*path == '.') {
                path += 1;
                nextNameTag = strchr(path, '.');
                if (nextNameTag == NULL) nextNameTag = end;
            }
        }
        else {
            if (cfg->m_type != CPE_CFG_TYPE_STRUCT) {
                return NULL;
            }

            if (nextSeqTag < nextNameTag) {
                cfg = cfg_do_find_cfg_from_struct(cfg, path, nextSeqTag);
                path = nextSeqTag;
                nextSeqTag = strchr(nextSeqTag, '[');
                if (nextSeqTag == NULL) nextSeqTag = end;
            }
            else if (nextNameTag < nextSeqTag) {
                cfg = cfg_do_find_cfg_from_struct(cfg, path, nextNameTag);
                path = nextNameTag + 1;
                nextNameTag = strchr(path, '.');
                if (nextNameTag == NULL) nextNameTag = end;
            }
            else {
                cfg = cfg_struct_find_cfg(cfg, path);
                path = end;
            }
        }
    }

    return cfg;
}
