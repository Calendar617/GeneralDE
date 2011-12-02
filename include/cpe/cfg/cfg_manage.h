#ifndef CPE_CFG_MANAGE_H
#define CPE_CFG_MANAGE_H
#include "cpe/utils/memory.h"
#include "cpe/utils/stream.h"
#include "cpe/utils/error.h"
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

cfg_t cfg_create(mem_allocrator_t alloc);
void cfg_free(cfg_t cfg);

/*struct operation*/
cfg_t cfg_struct_add_struct(cfg_t s, const char * name);
cfg_t cfg_struct_add_seq(cfg_t s, const char * name);
cfg_t cfg_struct_add_string(cfg_t s, const char * name, const char * value);
cfg_t cfg_struct_add_int8(cfg_t s, const char * name, int8_t v);
cfg_t cfg_struct_add_uint8(cfg_t s, const char * name, uint8_t v);
cfg_t cfg_struct_add_int16(cfg_t s, const char * name, int16_t v);
cfg_t cfg_struct_add_uint16(cfg_t s, const char * name, uint16_t v);
cfg_t cfg_struct_add_int32(cfg_t s, const char * name, int32_t v);
cfg_t cfg_struct_add_uint32(cfg_t s, const char * name, uint32_t v);
cfg_t cfg_struct_add_int64(cfg_t s, const char * name, int64_t v);
cfg_t cfg_struct_add_uint64(cfg_t s, const char * name, uint64_t v);
cfg_t cfg_struct_add_value(cfg_t s, const char * name, int typeId, const char * value);
cfg_t cfg_struct_add_value_auto(cfg_t s, const char * name, const char * value);

/*seq operation*/
cfg_t cfg_seq_add_struct(cfg_t s);
cfg_t cfg_seq_add_seq(cfg_t s);
cfg_t cfg_seq_add_string(cfg_t s, const char * value);
cfg_t cfg_seq_add_int8(cfg_t s, int8_t v);
cfg_t cfg_seq_add_uint8(cfg_t s, uint8_t v);
cfg_t cfg_seq_add_int16(cfg_t s, int16_t v);
cfg_t cfg_seq_add_uint16(cfg_t s, uint16_t v);
cfg_t cfg_seq_add_int32(cfg_t s, int32_t v);
cfg_t cfg_seq_add_uint32(cfg_t s, uint32_t v);
cfg_t cfg_seq_add_int64(cfg_t s, int64_t v);
cfg_t cfg_seq_add_uint64(cfg_t s, uint64_t v);
cfg_t cfg_seq_add_value(cfg_t s, int typeId, const char * value);
cfg_t cfg_seq_add_value_auto(cfg_t s, const char * value);

int cfg_read(cfg_t cfg, read_stream_t stream, cfg_read_policy_t policy, error_monitor_t em);
int cfg_write(write_stream_t stream, cfg_t cfg, error_monitor_t em);

#ifdef __cplusplus
}
#endif

#endif


