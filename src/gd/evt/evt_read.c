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

void gd_evt_set_from_string(gd_evt_t evt, const char * arg, const char * data) {
    //LPDRMETAENTRY entry = dr_meta_find_entry_by_path(LPDRMETA meta, const char* entryPath);

}

void gd_evt_set_from_int32(gd_evt_t evt, const char * arg, int32_t data) {
}

int8_t gd_evt_get_int8(gd_evt_t evt, const char * arg);
uint8_t gd_evt_get_uint8(gd_evt_t evt, const char * arg);
int16_t gd_evt_get_int16(gd_evt_t evt, const char * arg);
uint16_t gd_evt_get_uint16(gd_evt_t evt, const char * arg);
int32_t gd_evt_get_int32(gd_evt_t evt, const char * arg);
uint32_t gd_evt_get_uint32(gd_evt_t evt, const char * arg);
int64_t gd_evt_get_int64(gd_evt_t evt, const char * arg);
uint64_t gd_evt_get_uint64(gd_evt_t evt, const char * arg);
const char * gd_evt_get_string(gd_evt_t evt, const char * arg);
