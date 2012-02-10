#include <assert.h>
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "gd/tl/tl_action.h"
#include "gd/evt/evt_read.h"
#include "evt_internal_types.h"

gd_evt_t gd_evt_create(gd_evt_mgr_t evm, size_t attach_capacity, const char * typeName, ssize_t data_capacity, error_monitor_t em) {
    gd_tl_event_t tl_evt;
    LPDRMETA meta;
    gd_evt_t evt;

    assert(evm);
    
    meta = dr_lib_find_meta_by_name(evm->m_metalib, typeName);
    if (meta == NULL) {
        CPE_ERROR(em, "type %s not in metalib!", typeName);
        return NULL;
    }

    if (data_capacity < 0) {
        data_capacity = dr_meta_size(meta);
    }
    else if (data_capacity < dr_meta_size(meta)) {
        CPE_ERROR(em, "data_capacity %zd is to small to contain type %s!", data_capacity, typeName);
        return NULL;
    }

    tl_evt = gd_tl_event_create(
        evm->m_tl,
        sizeof(struct gd_evt) + data_capacity + attach_capacity);
    if (tl_evt == NULL) {
        CPE_ERROR(em, "create tl_event fail!");
        return NULL;
    }

    evt = gd_evt_cvt(tl_evt);

    evt->m_meta = meta;
    evt->m_data_capacity = data_capacity;

    dr_meta_set_defaults(evt + 1, meta);

    return evt;
}

int gd_evt_send(
    gd_evt_t evt,
    gd_tl_time_span_t delay,
    gd_tl_time_span_t span,
    int repeatCount)
{
    gd_tl_event_t tl_evt;

    assert(evt);

    tl_evt = gd_tl_event_from_data(evt);

    return gd_tl_event_send_ex(tl_evt, delay, span, repeatCount);
}

void * gd_evt_data(gd_evt_t evt) {
    return evt + 1;
}

size_t gd_evt_data_capacity(gd_evt_t evt) {
    return evt->m_data_capacity;
}

void * gd_evt_attach(gd_evt_t evt) {
    return ((char*)(evt + 1)) + evt->m_data_capacity;
}

gd_evt_t gd_evt_cvt(gd_tl_event_t tl_evt) {
    return (gd_evt_t)gd_tl_event_data(tl_evt);
}

size_t gd_evt_attach_capacity(gd_evt_t evt) {
    size_t total = gd_tl_event_capacity(gd_tl_event_from_data(evt));
    assert(total >= (sizeof(struct gd_evt) + evt->m_data_capacity));
    return total - sizeof(struct gd_evt) - evt->m_data_capacity;
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
