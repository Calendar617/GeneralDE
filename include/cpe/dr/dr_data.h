#ifndef CPE_DR_READ_H
#define CPE_DR_READ_H
#include "cpe/utils/buffer.h"
#include "cpe/utils/error.h"
#include "cpe/utils/stream.h"
#include "dr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int dr_entry_try_read_int8(int8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_uint8(uint8_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_int16(int16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_uint16(uint16_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_int32(int32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_uint32(uint32_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_int64(int64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_try_read_uint64(uint64_t * result, const void * input, LPDRMETAENTRY entry, error_monitor_t em);

int8_t dr_entry_read_int8(const void * input, LPDRMETAENTRY entry);
uint8_t dr_entry_read_uint8(const void * input, LPDRMETAENTRY entry);
int16_t dr_entry_read_int16(const void * input, LPDRMETAENTRY entry);
uint16_t dr_entry_read_uint16(const void * input, LPDRMETAENTRY entry);
int32_t dr_entry_read_int32(const void * input, LPDRMETAENTRY entry);
uint32_t dr_entry_read_uint32(const void * input, LPDRMETAENTRY entry);
int64_t dr_entry_read_int64(const void * input, LPDRMETAENTRY entry);
uint64_t dr_entry_read_uint64(const void * input, LPDRMETAENTRY entry);
const char * dr_entry_read_string(const void * input, LPDRMETAENTRY entry);

int dr_entry_print_to_stream(write_stream_t output, const void * input, LPDRMETAENTRY entry, error_monitor_t em);
const char * dr_entry_to_string(mem_buffer_t buf, const void * input, LPDRMETAENTRY entry);

int dr_entry_set_from_int8(void * output, int8_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_uint8(void * output, uint8_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_int16(void * output, int16_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_uint16(void * output, uint16_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_int32(void * output, int32_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_uint32(void * output, uint32_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_int64(void * output, int64_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_uint64(void * output, uint64_t input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_string(void * output, const char * input, LPDRMETAENTRY entry, error_monitor_t em);
int dr_entry_set_from_ctype(void * output, const void * input, int input_type, LPDRMETAENTRY entry, error_monitor_t em);

int dr_entry_set_defaults(void * inout, LPDRMETAENTRY entry);

void dr_meta_set_defaults(void * inout, LPDRMETA meta);

int dr_meta_try_read_int8(int8_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_uint8(uint8_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_int16(int16_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_uint16(uint16_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_int32(int32_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_uint32(uint32_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_int64(int64_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);
int dr_meta_try_read_uint64(uint64_t * result, const void * input, LPDRMETA meta, const char * entry, error_monitor_t em);

int8_t dr_meta_read_int8(const void * input, LPDRMETA meta, const char * entry);
uint8_t dr_meta_read_uint8(const void * input, LPDRMETA meta, const char * entry);
int16_t dr_meta_read_int16(const void * input, LPDRMETA meta, const char * entry);
uint16_t dr_meta_read_uint16(const void * input, LPDRMETA meta, const char * entry);
int32_t dr_meta_read_int32(const void * input, LPDRMETA meta, const char * entry);
uint32_t dr_meta_read_uint32(const void * input, LPDRMETA meta, const char * entry);
int64_t dr_meta_read_int64(const void * input, LPDRMETA meta, const char * entry);
uint64_t dr_meta_read_uint64(const void * input, LPDRMETA meta, const char * entry);
const char * dr_meta_read_string(const void * input, LPDRMETA meta, const char * entry);

#ifdef __cplusplus
}
#endif

#endif
