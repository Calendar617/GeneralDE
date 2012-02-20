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

int dr_cfg_read_entry(
    char * all_buf, size_t all_capacity, char * entry_buf, size_t entry_capacity,
    cfg_t cfg, LPDRMETA meta, LPDRMETAENTRY entry, int policy, error_monitor_t em);

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

void dr_cfg_read_array_set_dft(size_t count, size_t max_count, size_t element_size, char * write_pos, LPDRMETA meta, LPDRMETAENTRY entry) {
    const void * dftValue;

    dftValue = dr_entry_dft_value(entry);
    if (dftValue) {
        while(count < max_count) {
            memcpy(write_pos, dftValue, element_size);
            write_pos += element_size;
            ++count;
        }
    }
    else {
        bzero(write_pos, (max_count - count) * element_size);
    }
}

int dr_cfg_read_entry(
    char * all_buf, size_t all_capacity,
    char * entry_buf, size_t entry_capacity,
    cfg_t cfg, LPDRMETA meta, LPDRMETAENTRY entry, int policy, error_monitor_t em)
{
    cfg_it_t itemIt;
    cfg_t item;

    assert(entry);

    if (entry->m_array_count == 1) {
        return dr_cfg_read_entry_one(
            entry_buf,
            entry_capacity,
            cfg, meta, entry, policy, em);
    }
    else {
        LPDRMETAENTRY refer;
        int count;
        int max_count;
        char * write_pos;
        size_t element_size = dr_entry_element_size(entry);
        if (element_size == 0) {
            CPE_ERROR(
                em, "read from %s: read %s.%s, element size is unknown!",
                cfg_name(cfg),
                dr_meta_name(meta), cfg_name(cfg));
            return entry->m_unitsize;
        }

        count = 0;
        write_pos = entry_buf;
        max_count = entry->m_array_count;

        if (max_count == 0) {
            max_count = entry_capacity / element_size;
        }
        else {
            if (max_count * element_size > entry_capacity) {
                CPE_ERROR(
                    em,
                    "process %s.%s, array element overflow, require %zd, capacity is %zd!",
                    dr_meta_name(meta), dr_entry_name(entry),
                    max_count * element_size, entry_capacity);
                max_count = entry_capacity / element_size;
            }
        }

        cfg_it_init(&itemIt, cfg);
        while((item = cfg_it_next(&itemIt)) && count < max_count) {
            dr_cfg_read_entry_one(write_pos, element_size, item, meta, entry, policy, em);
            write_pos += element_size;
            ++count;
        }

        refer = dr_entry_array_refer_entry(entry);
        if (refer) {
            dr_entry_set_from_int32(
                all_buf + entry->m_array_refer_data_start_pos,
                count,
                refer,
                em);
        }
        else {
            dr_cfg_read_array_set_dft(count, max_count, element_size, write_pos, meta, entry);
        }

        return count * element_size;
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
    cfg_t item, next;
    int size;
    size_t total_capacity;

    assert(cfg);

    size = 0;

    cfg_it_init(&itemIt, cfg);

    for(item = cfg_it_next(&itemIt), next = cfg_it_next(&itemIt);
        item && capacity > 0;
        item = next, next = cfg_it_next(&itemIt))
    {
        size_t entry_size;
        size_t entry_capacity;
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

        entry_capacity = entry->m_unitsize;
        if (entry_capacity > capacity || next == 0) entry_capacity = capacity;

        entry_size = dr_cfg_read_entry(buf, total_capacity, buf + entry->m_data_start_pos, entry_capacity, item, meta, entry, policy, em);
        capacity -= entry_capacity;
        
        if (entry->m_data_start_pos + entry_size > size) {
            size = entry->m_data_start_pos + entry_size;
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
