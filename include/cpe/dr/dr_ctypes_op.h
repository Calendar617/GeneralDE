#ifndef CPE_DR_CTYPES_READ_H
#define CPE_DR_CTYPES_READ_H
#include "cpe/utils/error.h"
#include "cpe/utils/stream.h"
#include "dr_ctypes_info.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_ctype_try_read_int8(int8_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_uint8(uint8_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_int16(int16_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_uint16(uint16_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_int32(int32_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_uint32(uint32_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_int64(int64_t * result, const void * input, int type, error_monitor_t em);
int dr_ctype_try_read_uint64(uint64_t * result, const void * input, int type, error_monitor_t em);

int8_t dr_ctype_read_int8(const void * input, int type);
uint8_t dr_ctype_read_uint8(const void * input, int type);
int16_t dr_ctype_read_int16(const void * input, int type);
uint16_t dr_ctype_read_uint16(const void * input, int type);
int32_t dr_ctype_read_int32(const void * input, int type);
uint32_t dr_ctype_read_uint32(const void * input, int type);
int64_t dr_ctype_read_int64(const void * input, int type);
uint64_t dr_ctype_read_uint64(const void * input, int type);

int dr_ctype_print_to_stream(write_stream_t output, const void * input, int type, error_monitor_t em);
int dr_ctype_set_from_string(void * output, int type, const char * input, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
