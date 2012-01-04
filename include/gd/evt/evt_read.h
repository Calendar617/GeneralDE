#ifndef GD_DP_EVT_READ_H
#define GD_DP_EVT_READ_H
#include "cpe/utils/error.h"
#include "cpe/utils/memory.h"
#include "evt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_evt_t gd_evt_create(gd_evt_mgr_t evm, const char * typeName, error_monitor_t em);

const char * gd_evt_type(gd_evt_t evt);
void * gd_evt_data(gd_evt_t evt);
LPDRMETA gd_evt_meta(gd_evt_t evt);

int gd_evt_set_from_string(gd_evt_t evt, const char * arg, const char * data, error_monitor_t em);
int gd_evt_set_from_int32(gd_evt_t evt, const char * arg, int32_t data, error_monitor_t em);

int gd_evt_try_get_int8(int8_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_uint8(uint8_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_int16(int16_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_uint16(uint16_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_int32(int32_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_uint32(uint32_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_int64(int64_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);
int gd_evt_try_get_uint64(uint64_t * result, gd_evt_t evt, const char * arg, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
