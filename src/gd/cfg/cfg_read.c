#include "cpe/dr/dr_ctypes_op.h"
#include "gd/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

void * gd_cfg_data(gd_cfg_t cfg) {
    if (cfg == NULL || !gd_cfg_is_value(cfg)) {
        return NULL;
    }

    return (void*)(cfg + 1);
}

char * gd_cfg_name(gd_cfg_t cfg) {
    if (cfg == NULL || cfg->m_parent == NULL) return "";

    return gd_cfg_to_struct_item(cfg)->m_name;
}

int gd_cfg_type(gd_cfg_t cfg) {
    if (cfg == NULL) return CPE_DR_TYPE_UNKOWN;

    return cfg->m_type;
}

gd_cfg_t gd_cfg_parent(gd_cfg_t cfg) {
    if (cfg == NULL) return NULL;

    return cfg->m_parent;
}

int gd_cfg_is_value(gd_cfg_t cfg) {
    return cfg->m_type > CPE_DR_TYPE_COMPOSITE;
}

#define GD_CFG_GEN_READ_TYPE(__type)                            \
__type ## _t gd_cfg_get_  ## __type(                            \
        gd_cfg_t cfg, const char * path, __type ## _t dft) {    \
    __type ## _t rv;                                            \
    gd_cfg_t at = gd_cfg_find_cfg(cfg, path);                   \
    if (at == NULL || !gd_cfg_is_value(at) ) return dft;        \
    return dr_ctype_try_read_ ## __type(                        \
        &rv, gd_cfg_data(at), at->m_type, NULL) == 0            \
        ? rv                                                    \
        : dft;                                                  \
}

GD_CFG_GEN_READ_TYPE(int8);
GD_CFG_GEN_READ_TYPE(uint8);
GD_CFG_GEN_READ_TYPE(int16);
GD_CFG_GEN_READ_TYPE(uint16);
GD_CFG_GEN_READ_TYPE(int32);
GD_CFG_GEN_READ_TYPE(uint32);
GD_CFG_GEN_READ_TYPE(int64);
GD_CFG_GEN_READ_TYPE(uint64);

const char * gd_cfg_get_string(gd_cfg_t cfg, const char * path, const char * dft) {
    gd_cfg_t at = gd_cfg_find_cfg(cfg, path);
    if (at == NULL) return dft;

    return at->m_type == CPE_DR_TYPE_STRING
        ? (const char *)gd_cfg_data(at)
        : dft;
}

gd_cfg_t gd_cfg_find_cfg(gd_cfg_t cfg, const char * path) {
    return NULL;
}
