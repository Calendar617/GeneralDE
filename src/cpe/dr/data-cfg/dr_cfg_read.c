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

void dr_cfg_read_i(
    char * buf,
    size_t capacity,
    cfg_t cfg,
    LPDRMETA meta,
    error_monitor_t em)
{
    cfg_it_t itemIt;
    cfg_t item;

    assert(cfg);

    if (meta == NULL) {
        CPE_ERROR(em, "read from %s: meta is empty!", cfg_name(cfg));
        return;
    }

    if (meta->m_type != CPE_DR_TYPE_STRUCT) {
        CPE_ERROR(
            em, "read from %s: not support read to %s!",
            cfg_name(cfg), dr_type_name(meta->m_type));
        return;
    }

    cfg_it_init(&itemIt, cfg);
    while((item = cfg_it_next(&itemIt))) {
        LPDRMETAENTRY entry = dr_meta_find_entry_by_name(meta, cfg_name(item));
        if (entry == NULL) {
            CPE_WARNING(
                em, "read from %s: %s have no entry %s, ignore!",
                cfg_name(cfg), dr_meta_name(meta), cfg_name(item));
            continue;
        }

        if (entry->m_data_start_pos + entry->m_unitsize > capacity) {
            CPE_WARNING(
                em, "read from %s: read %s.%s, size overflow, require %d, but only %d!",
                cfg_name(cfg),
                dr_meta_name(meta), cfg_name(item),
                entry->m_data_start_pos + entry->m_unitsize,
                capacity);
            continue;
        }

        if (entry->m_type <= CPE_DR_TYPE_COMPOSITE) {
            dr_cfg_read_i(
                buf + entry->m_data_start_pos,
                entry->m_unitsize,
                item,
                dr_entry_ref_meta(entry),
                em);
        }
        else {
            dr_entry_set_from_ctype(
                buf + entry->m_data_start_pos,
                cfg_data(item),
                cfg_type(item),
                entry,
                em);
        }
    }
}

int dr_cfg_read(
    void * buf,
    size_t capacity,
    cfg_t cfg,
    LPDRMETA meta,
    error_monitor_t em)
{
    int ret = 0;

    if (em) {
        CPE_DEF_ERROR_MONITOR_ADD(logError, em, cpe_error_save_last_errno, &ret);
        dr_cfg_read_i((char *)buf, capacity, cfg, meta, em);
        CPE_DEF_ERROR_MONITOR_REMOVE(logError, em);
    }
    else {
        CPE_DEF_ERROR_MONITOR(logError, cpe_error_save_last_errno, &ret);
        dr_cfg_read_i((char *)buf, capacity, cfg, meta, &logError);
    }

    return ret;
}
