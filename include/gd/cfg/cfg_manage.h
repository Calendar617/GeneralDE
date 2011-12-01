#ifndef GD_CFG_MANAGE_H
#define GD_CFG_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/stream.h"
#include "cpe/utils/error.h"
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_cfg_t gd_cfg_create(mem_allocrator_t alloc);
void gd_cfg_free(gd_cfg_t cfg);

/*struct operation*/
gd_cfg_t gd_cfg_struct_add_struct(gd_cfg_t s, const char * name);
gd_cfg_t gd_cfg_struct_add_seq(gd_cfg_t s, const char * name);
gd_cfg_t gd_cfg_struct_add_string(gd_cfg_t s, const char * name, const char * value);
gd_cfg_t gd_cfg_struct_add_int8(gd_cfg_t s, const char * name, int8_t v);
gd_cfg_t gd_cfg_struct_add_uint8(gd_cfg_t s, const char * name, uint8_t v);
gd_cfg_t gd_cfg_struct_add_int16(gd_cfg_t s, const char * name, int16_t v);
gd_cfg_t gd_cfg_struct_add_uint16(gd_cfg_t s, const char * name, uint16_t v);
gd_cfg_t gd_cfg_struct_add_int32(gd_cfg_t s, const char * name, int32_t v);
gd_cfg_t gd_cfg_struct_add_uint32(gd_cfg_t s, const char * name, uint32_t v);
gd_cfg_t gd_cfg_struct_add_int64(gd_cfg_t s, const char * name, int64_t v);
gd_cfg_t gd_cfg_struct_add_uint64(gd_cfg_t s, const char * name, uint64_t v);

/*seq operation*/
gd_cfg_t gd_cfg_seq_add_struct(gd_cfg_t s);
gd_cfg_t gd_cfg_seq_add_seq(gd_cfg_t s);
gd_cfg_t gd_cfg_seq_add_string(gd_cfg_t s, const char * value);
gd_cfg_t gd_cfg_seq_add_int8(gd_cfg_t s, int8_t v);
gd_cfg_t gd_cfg_seq_add_uint8(gd_cfg_t s, uint8_t v);
gd_cfg_t gd_cfg_seq_add_int16(gd_cfg_t s, int16_t v);
gd_cfg_t gd_cfg_seq_add_uint16(gd_cfg_t s, uint16_t v);
gd_cfg_t gd_cfg_seq_add_int32(gd_cfg_t s, int32_t v);
gd_cfg_t gd_cfg_seq_add_uint32(gd_cfg_t s, uint32_t v);
gd_cfg_t gd_cfg_seq_add_int64(gd_cfg_t s, int64_t v);
gd_cfg_t gd_cfg_seq_add_uint64(gd_cfg_t s, uint64_t v);

int gd_cfg_read(gd_cfg_t cfg, read_stream_t stream, error_monitor_t em);
int gd_cfg_write(write_stream_t stream, gd_cfg_t cfg, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif


