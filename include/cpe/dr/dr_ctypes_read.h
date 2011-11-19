#ifndef CPE_DR_CTYPES_READ_H
#define CPE_DR_CTYPES_READ_H
#include "cpe/utils/error.h"
#include "dr_ctypes_info.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_try_read_int8(int8_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_uint8(uint8_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_int16(int16_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_uint16(uint16_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_int32(int32_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_uint32(uint32_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_int64(int64_t * result, const void * input, int type, error_monitor_t em);
int dr_try_read_uint64(uint64_t * result, const void * input, int type, error_monitor_t em);

int8_t dr_read_int8(const void * input, int type);
uint8_t dr_read_uint8(const void * input, int type);
int16_t dr_read_int16(const void * input, int type);
uint16_t dr_read_uint16(const void * input, int type);
int32_t dr_read_int32(const void * input, int type);
uint32_t dr_read_uint32(const void * input, int type);
int64_t dr_read_int64(const void * input, int type);
uint64_t dr_read_uint64(const void * input, int type);

#ifdef __cplusplus
}
#endif

#endif
