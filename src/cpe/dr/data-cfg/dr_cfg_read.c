#include <assert.h>
#include "cpe/pal/pal_string.h"
#include "cpe/pal/pal_strings.h"
#include "cpe/cfg/cfg_read.h"
#include "cpe/dr/dr_cfg.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "../dr_internal_types.h"
#include "../dr_ctype_ops.h"

int dr_cfg_read_entry(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, LPDRMETAENTRY entry, int policy, error_monitor_t em);
int dr_cfg_read_struct(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, int policy, error_monitor_t em);
int dr_cfg_read_union(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, int policy, error_monitor_t em);

int dr_cfg_read_entry_one(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, LPDRMETAENTRY entry, int policy, error_monitor_t em) {
    assert(entry);

    if (entry->m_type == CPE_DR_TYPE_STRUCT) {
        LPDRMETA ref;
        ref = dr_entry_ref_meta(entry);
        if (ref == NULL) {
            CPE_ERROR(
                em, "read from %s: read %s.%s, entry have no ref meta!",
                cfg_name(cfg),
                dr_meta_name(meta), cfg_name(cfg));
            return -1;
        }

        return dr_cfg_read_struct(buf, capacity, cfg, ref, policy, em);
    }
    else if (entry->m_type == CPE_DR_TYPE_UNION) {
        LPDRMETA ref;
        ref = dr_entry_ref_meta(entry);
        if (ref == NULL) {
            CPE_ERROR(
                em, "read from %s: read %s.%s, entry have no ref meta!",
                cfg_name(cfg),
                dr_meta_name(meta), cfg_name(cfg));
            return -1;
        }

        return dr_cfg_read_union(buf, capacity, cfg, ref, policy, em);
    }
    else {
        dr_entry_set_from_ctype(
            buf,
            cfg_data(cfg),
            cfg_type(cfg),
            entry,
            em);
        return entry->m_unitsize;
    }
}

size_t dr_cfg_read_calc_element_size(LPDRMETA meta, LPDRMETAENTRY entry, error_monitor_t em) {
    if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
        LPDRMETA refMeta = dr_entry_ref_meta(entry);
        if (refMeta == NULL) {
            CPE_ERROR(
                em, "process %s.%s, ref meta not exist!",
                dr_meta_name(meta), dr_entry_name(entry));
            return 0;
        }

        return dr_meta_size(refMeta);
    }
    else {
        const struct tagDRCTypeInfo * typeInfo;
        typeInfo = dr_find_ctype_info_by_type(entry->m_type);
        if (typeInfo == NULL) {
            CPE_ERROR(
                em, "process %s.%s, type %d is unknown!",
                dr_meta_name(meta), dr_entry_name(entry), entry->m_type);
            return 0;
        }

        if (typeInfo->m_size <= 0) {
            CPE_ERROR(
                em, "process %s.%s, type %d size is invalid!",
                dr_meta_name(meta), dr_entry_name(entry), entry->m_type);
            return 0;
        }

        return typeInfo->m_size;
    }
}

void dr_cfg_read_array_set_dft(size_t count, size_t elementSize, char * buf, size_t capacity, LPDRMETA meta, LPDRMETAENTRY entry) {
    const void * dftValue;
    char * writePos;

    if (count >= entry->m_array_count) return;

    writePos = buf + entry->m_data_start_pos + (count * elementSize);
    dftValue = dr_entry_dft_value(entry);
    if (dftValue) {
        while(count < entry->m_array_count) {
            size_t requiredSize;

            requiredSize = (writePos - buf) + elementSize;
            if (requiredSize > capacity) {
                break;
            }

            memcpy(writePos, dftValue, elementSize);
            writePos += elementSize;
            ++count;
        }
    }
    else {
        size_t totalSize = entry->m_array_count * elementSize;
        if (totalSize > capacity) totalSize = capacity;

        ssize_t writeSize = ((ssize_t)totalSize) - ((size_t)(count * elementSize));

        bzero(writePos, writeSize);
    }
}


int dr_cfg_read_entry(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, LPDRMETAENTRY entry, int policy, error_monitor_t em) {
    cfg_it_t itemIt;
    cfg_t item;

    assert(entry);

    if (entry->m_data_start_pos + entry->m_unitsize > capacity) {
        CPE_WARNING(
            em, "read from %s: read %s.%s, size overflow, require %d, but only %zd!",
            cfg_name(cfg),
            dr_meta_name(meta), cfg_name(cfg),
            entry->m_data_start_pos + entry->m_unitsize,
            capacity);
        return -1;
    }

    if (entry->m_array_count == 1) {
        return dr_cfg_read_entry_one(
            buf + entry->m_data_start_pos,
            entry->m_unitsize,
            cfg, meta, entry, policy, em);
    }
    else if (entry->m_array_count == 0) {
        LPDRMETAENTRY refer;
        int count;
        char * writePos;
        size_t elementSize = dr_cfg_read_calc_element_size(meta, entry, em);
        if (elementSize == 0) return entry->m_unitsize;

        cfg_it_init(&itemIt, cfg);
        count = 0;
        writePos = buf + entry->m_data_start_pos;
        while((item = cfg_it_next(&itemIt)) && count < entry->m_array_count) {
            size_t requiredSize;

            requiredSize = (writePos - buf) + elementSize;
            if (requiredSize > capacity) break;

            dr_cfg_read_entry_one(writePos, elementSize, item, meta, entry, policy, em);
            
            writePos += elementSize;
            ++count;
        }

        refer = dr_entry_array_refer_entry(entry);
        if (refer) {
            dr_entry_set_from_int32(
                buf + entry->m_array_refer_data_start_pos,
                count,
                refer,
                em);
        }

        return writePos - buf;
    }
    else {
        LPDRMETAENTRY refer;
        int count;
        char * writePos;
        size_t elementSize = dr_cfg_read_calc_element_size(meta, entry, em);
        if (elementSize == 0) return entry->m_unitsize;

        cfg_it_init(&itemIt, cfg);
        count = 0;
        writePos = buf + entry->m_data_start_pos;
        while((item = cfg_it_next(&itemIt)) && count < entry->m_array_count) {
            size_t requiredSize;

            requiredSize = (writePos - buf) + elementSize;
            if (requiredSize > capacity) {
                CPE_ERROR(
                    em, "process %s.%s, array element %d overflow, require %zd, capacity is %zd!",
                    dr_meta_name(meta), dr_entry_name(entry), count,
                    requiredSize, capacity);
                break;
            }

            dr_cfg_read_entry_one(writePos, elementSize, item, meta, entry, policy, em);
            
            writePos += elementSize;
            ++count;
        }

        refer = dr_entry_array_refer_entry(entry);
        if (refer) {
            dr_entry_set_from_int32(
                buf + entry->m_array_refer_data_start_pos,
                count,
                refer,
                em);
        }
        else {
            dr_cfg_read_array_set_dft(count, elementSize, buf, capacity, meta, entry);
        }

        return writePos - buf;
    }
}

int dr_cfg_read_union(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, int policy, error_monitor_t em) {
    CPE_ERROR(
        em, "read from %s: %s is union, not support read union!",
        cfg_name(cfg), dr_meta_name(meta));
    return meta->m_data_size;
}

int dr_cfg_read_struct(char * buf, size_t capacity, cfg_t cfg, LPDRMETA meta, int policy, error_monitor_t em) {
    cfg_it_t itemIt;
    cfg_t item;
    int size;

    assert(cfg);

    size = meta->m_data_size;

    cfg_it_init(&itemIt, cfg);
    while((item = cfg_it_next(&itemIt))) {
        size_t itemSize;
        LPDRMETAENTRY entry;

        entry = dr_meta_find_entry_by_name(meta, cfg_name(item));
        if (entry == NULL) {
            if (policy | DR_CFG_READ_CHECK_NOT_EXIST_ATTR) {
                CPE_WARNING(
                    em, "read from %s: %s have no entry %s, ignore!",
                    cfg_name(cfg), dr_meta_name(meta), cfg_name(item));
            }
            continue;
        }

        itemSize = dr_cfg_read_entry(buf, capacity, item, meta, entry, policy, em);

        if (itemSize > 0 && itemSize + entry->m_data_start_pos > size) {
            size = itemSize + entry->m_data_start_pos;
        }
    }

    return size;
}

int dr_cfg_read_i(
    char * buf,
    size_t capacity,
    cfg_t cfg,
    LPDRMETA meta,
    int policy,
    error_monitor_t em)
{
    if (meta == NULL) {
        CPE_ERROR(em, "read from %s: meta is empty!", cfg_name(cfg));
        return 0;
    }

    if (meta->m_type == CPE_DR_TYPE_STRUCT) {
        return dr_cfg_read_struct(buf, capacity, cfg, meta, policy, em);
    }
    else {
        assert(meta->m_type == CPE_DR_TYPE_UNION);
        return dr_cfg_read_union(buf, capacity, cfg, meta, policy, em);
    }
}

int dr_cfg_read(
    void * buf,
    size_t capacity,
    cfg_t cfg,
    LPDRMETA meta,
    int policy,
    error_monitor_t em)
{
    int ret = 0;
    int size = 0;

    if (cfg == NULL) {
        CPE_ERROR(em, "dr_cfg_read: input cfg is null!");
        return -1;
    }

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        size = dr_cfg_read_i((char *)buf, capacity, cfg, meta, policy, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        size = dr_cfg_read_i((char *)buf, capacity, cfg, meta, policy, &logError);
    }

    return ret ? ret : size;
}
