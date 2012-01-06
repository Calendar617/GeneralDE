#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "gd/tl/tl_action.h"
#include "gd/evt/evt_read.h"
#include "evt_internal_types.h"

gd_evt_t gd_evt_create(gd_evt_mgr_t evm, const char * typeName, error_monitor_t em) {
    gd_tl_event_t tl_evt;
    LPDRMETA meta;
    gd_evt_t evt;

    assert(evm);
    
    meta = dr_lib_find_meta_by_name(evm->m_metalib, typeName);
    if (meta == NULL) {
        CPE_ERROR(em, "type %s not in metalib!", typeName);
        return NULL;
    }

    tl_evt = gd_tl_event_create(
        evm->m_tl,
        sizeof(struct gd_evt) + dr_meta_size(meta));
    if (tl_evt == NULL) {
        CPE_ERROR(em, "create tl_event fail!");
        return NULL;
    }

    evt = (gd_evt_t)gd_tl_event_data(tl_evt);

    evt->m_meta = meta;

    dr_meta_set_defaults(evt + 1, meta);

    return evt;
}

void * gd_evt_data(gd_evt_t evt) {
    return evt + 1;
}

LPDRMETA gd_evt_meta(gd_evt_t evt) {
    return evt->m_meta;
}

const char * gd_evt_type(gd_evt_t evt) {
    return dr_meta_name(evt->m_meta);
}

int gd_evt_set_from_string(gd_evt_t evt, const char * arg, const char * data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_set_from_string(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_int8(gd_evt_t evt, const char * arg, int8_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_set_from_int8(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_uint8(gd_evt_t evt, const char * arg, uint8_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        return -1;
    }

    return dr_entry_set_from_uint8(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_int16(gd_evt_t evt, const char * arg, int16_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_set_from_int16(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_uint16(gd_evt_t evt, const char * arg, uint16_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        return -1;
    }

    return dr_entry_set_from_uint16(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_int32(gd_evt_t evt, const char * arg, int32_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_set_from_int32(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_uint32(gd_evt_t evt, const char * arg, uint32_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        return -1;
    }

    return dr_entry_set_from_uint32(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_int64(gd_evt_t evt, const char * arg, int64_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_set_from_int64(gd_evt_data(evt), data, entry, em);
}

int gd_evt_set_from_uint64(gd_evt_t evt, const char * arg, uint64_t data, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        return -1;
    }

    return dr_entry_set_from_uint64(gd_evt_data(evt), data, entry, em);
}


int gd_evt_try_get_int8(int8_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_int8(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_uint8(uint8_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_uint8(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_int16(int16_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_int16(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_uint16(uint16_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_uint16(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_int32(int32_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_int32(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_uint32(uint32_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_uint32(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_int64(int64_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_int64(result, gd_evt_data(evt), entry, em);
}

int gd_evt_try_get_uint64(uint64_t * result, gd_evt_t evt, const char * arg, error_monitor_t em) {
    LPDRMETAENTRY entry = dr_meta_find_entry_by_name(evt->m_meta, arg);
    if (entry == NULL) {
        CPE_ERROR(em, "event %s have no arg %s!", gd_evt_type(evt), arg);
        return -1;
    }

    return dr_entry_try_read_uint64(result, gd_evt_data(evt), entry, em);
}
