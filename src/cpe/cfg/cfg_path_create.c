#include <assert.h>
#include <string.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/utils/string_utils.h"
#include "cpe/dr/dr_ctypes_op.h"
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "cfg_internal_types.h"
#include "cfg_internal_ops.h"

static cfg_t cfg_check_or_create_sub(cfg_t cfg, int type, const char * next_cfg_str) {
    if(cfg->m_type == CPE_CFG_TYPE_STRUCT) {
        if (type == CPE_CFG_TYPE_STRUCT) {
            return cfg_struct_add_struct(cfg, next_cfg_str, cfg_merge_use_exist);
        }
        else if (type == CPE_CFG_TYPE_SEQUENCE) {
            return cfg_struct_add_seq(cfg, next_cfg_str, cfg_merge_use_exist);
        }
        else {
            return NULL;
        }
    }
    else if (cfg->m_type == CPE_CFG_TYPE_SEQUENCE) {
        if (next_cfg_str[0] == 0) {
            if (type == CPE_CFG_TYPE_STRUCT) {
                return cfg_seq_add_struct(cfg);
            }
            else if (type == CPE_CFG_TYPE_SEQUENCE) {
                return cfg_seq_add_seq(cfg);
            }
            else {
                return NULL;
            }
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

static cfg_t cfg_check_or_create(cfg_t cfg, const char * path, error_monitor_t em, char * buf, size_t buf_capacity) {
    const char * root = path;
    const char * end = path + strlen(path);
    const char * nextSeqTag = strchr(path, '[');
    const char * nextNameTag = strchr(path, '.');
    const char * next_cfg_str;

    if (nextSeqTag == NULL) nextSeqTag = end;
    if (nextNameTag == NULL) nextNameTag = end;

    next_cfg_str = NULL;

    while(cfg && path < end) {
        if (path[0] == '[') {
			const char * seqEndTag;
            seqEndTag = strchr(nextSeqTag, ']');
            if (seqEndTag == NULL) {
                CPE_ERROR(em, "cfg_check_or_create: path=%s, pos=%d: seq tag not closed", root, (int)(nextSeqTag - root));
                return NULL;
            }

            if (next_cfg_str) {
                cfg = cfg_check_or_create_sub(cfg, CPE_CFG_TYPE_SEQUENCE, next_cfg_str);
                if (cfg == NULL) break;
            }

            next_cfg_str = cpe_str_dup_range(buf, buf_capacity, path + 1, seqEndTag);
            if (next_cfg_str == NULL) {
                CPE_ERROR(em, "cfg_check_or_create: path=%s, pos=%d: name too long, capacity=%d", root, (int)(nextSeqTag - root), (int)buf_capacity);
                return NULL;
            }

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
            const char * next_cfg_str_begin;

            if (next_cfg_str) {
                cfg = cfg_check_or_create_sub(cfg, CPE_CFG_TYPE_STRUCT, next_cfg_str);
                if (cfg == NULL) break;
            }

            next_cfg_str_begin = path;

            if (nextSeqTag < nextNameTag) {
                path = nextSeqTag;
                nextSeqTag = strchr(nextSeqTag, '[');
                if (nextSeqTag == NULL) nextSeqTag = end;
            }
            else if (nextNameTag < nextSeqTag) {
                path = nextNameTag + 1;
                nextNameTag = strchr(path, '.');
                if (nextNameTag == NULL) nextNameTag = end;
            }
            else {
                path = end;
            }

            next_cfg_str = cpe_str_dup_range(buf, buf_capacity, next_cfg_str_begin, path);
            if (next_cfg_str == NULL) {
                CPE_ERROR(em, "cfg_check_or_create: path=%s, pos=%d: name too long, capacity=%d", root, (int)(path - root), (int)buf_capacity);
                return NULL;
            }
        }
    }

    return next_cfg_str == NULL ? NULL : cfg;
}

cfg_t cfg_add_struct(cfg_t c, const char * path, error_monitor_t em) {
    char buf[256];
    cfg_t last_cfg;

    last_cfg = cfg_check_or_create(c, path, em, buf, sizeof(buf));
    if (last_cfg == NULL) return NULL;

    return cfg_check_or_create_sub(last_cfg, CPE_CFG_TYPE_STRUCT, buf);
}

cfg_t cfg_add_seq(cfg_t c, const char * path, error_monitor_t em) {
    char buf[256];
    cfg_t last_cfg;

    last_cfg = cfg_check_or_create(c, path, em, buf, sizeof(buf));
    if (last_cfg == NULL) return NULL;

    return cfg_check_or_create_sub(last_cfg, CPE_CFG_TYPE_SEQUENCE, buf);
}

cfg_t cfg_add_string(cfg_t c, const char * path, const char * value, error_monitor_t em) {
    char buf[256];
    cfg_t last_cfg;

    last_cfg = cfg_check_or_create(c, path, em, buf, sizeof(buf));
    if (last_cfg == NULL) return NULL;

    if (last_cfg->m_type == CPE_CFG_TYPE_SEQUENCE) {
        
    }
}

cfg_t cfg_add_int8(cfg_t c, const char * path, int8_t v, error_monitor_t em);
cfg_t cfg_add_uint8(cfg_t c, const char * path, uint8_t v, error_monitor_t em);
cfg_t cfg_add_int16(cfg_t c, const char * path, int16_t v, error_monitor_t em);
cfg_t cfg_add_uint16(cfg_t c, const char * path, uint16_t v, error_monitor_t em);
cfg_t cfg_add_int32(cfg_t c, const char * path, int32_t v, error_monitor_t em);
cfg_t cfg_add_uint32(cfg_t c, const char * path, uint32_t v, error_monitor_t em);
cfg_t cfg_add_int64(cfg_t c, const char * path, int64_t v, error_monitor_t em);
cfg_t cfg_add_uint64(cfg_t c, const char * path, uint64_t v, error_monitor_t em);
cfg_t cfg_add_float(cfg_t c, const char * path, float v, error_monitor_t em);
cfg_t cfg_add_double(cfg_t c, const char * path, double v, error_monitor_t em);
cfg_t cfg_add_value_from_string(cfg_t c, const char * path, int typeId, const char * value, error_monitor_t em);
cfg_t cfg_add_value_from_string_auto(cfg_t c, const char * path, const char * value, error_monitor_t em);
cfg_t cfg_add_value_from_binary(cfg_t c, const char * path, int typeId, const void * value, error_monitor_t em);
