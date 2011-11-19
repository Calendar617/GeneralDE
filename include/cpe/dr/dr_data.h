#ifndef CPE_DR_READ_H
#define CPE_DR_READ_H
#include "cpe/utils/buffer.h"
#include "cpe/utils/error.h"
#include "cpe/utils/stream.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_try_read_int8(int8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_uint8(uint8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_int16(int16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_uint16(uint16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_int32(int32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_uint32(uint32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_int64(int64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_try_read_uint64(uint64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);

int dr_write_from_string(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em);

int8_t dr_read_int8(const void * input, LPDRMETAENTRY entry);
uint8_t dr_read_uint8(const void * input, LPDRMETAENTRY entry);
int16_t dr_read_int16(const void * input, LPDRMETAENTRY entry);
uint16_t dr_read_uint16(const void * input, LPDRMETAENTRY entry);
int32_t dr_read_int32(const void * input, LPDRMETAENTRY entry);
uint32_t dr_read_uint32(const void * input, LPDRMETAENTRY entry);
int64_t dr_read_int64(const void * input, LPDRMETAENTRY entry);
uint64_t dr_read_uint64(const void * input, LPDRMETAENTRY entry);

int dr_print_to_stream(write_stream_t output, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
const char * dr_to_string(mem_buffer_t buf, const void * input, LPDRMETAENTRY entry);

int dr_set_from_string(void * output, LPDRMETAENTRY entry, const char * input, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif
