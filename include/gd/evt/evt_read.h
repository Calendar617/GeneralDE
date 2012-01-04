#ifndef GD_DP_EVT_READ_H
#define GD_DP_EVT_READ_H
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "evt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_evt_t gd_evt_create(gd_evt_mgr_t evm, const char * typeName, error_monitor_t em);

void * gd_evt_data(gd_evt_t evt);
LPDRMETA gd_evt_meta(gd_evt_t evt);

void gd_evt_set_from_string(gd_evt_t evt, const char * arg, const char * data);
void gd_evt_set_from_int32(gd_evt_t evt, const char * arg, int32_t data);

int8_t gd_evt_get_int8(gd_evt_t evt, const char * arg);
uint8_t gd_evt_get_uint8(gd_evt_t evt, const char * arg);
int16_t gd_evt_get_int16(gd_evt_t evt, const char * arg);
uint16_t gd_evt_get_uint16(gd_evt_t evt, const char * arg);
int32_t gd_evt_get_int32(gd_evt_t evt, const char * arg);
uint32_t gd_evt_get_uint32(gd_evt_t evt, const char * arg);
int64_t gd_evt_get_int64(gd_evt_t evt, const char * arg);
uint64_t gd_evt_get_uint64(gd_evt_t evt, const char * arg);
const char * gd_evt_get_string(gd_evt_t evt, const char * arg);

#ifdef __cplusplus
}
#endif

#endif
