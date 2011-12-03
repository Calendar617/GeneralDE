#ifndef CPE_CFG_READ_H
#define CPE_CFG_READ_H
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*basic operations*/
const char * cfg_name(cfg_t cfg);
void * cfg_data(cfg_t cfg);
int cfg_type(cfg_t cfg);
cfg_t cfg_parent(cfg_t cfg);
int cfg_type_is_value(int type);
int cfg_is_value(cfg_t cfg);
cfg_t cfg_find_cfg(cfg_t cfg, const char * path);

/*get data operation*/
int8_t cfg_as_int8(cfg_t cfg, int8_t dft);
uint8_t cfg_as_uint8(cfg_t cfg, uint8_t dft);
int16_t cfg_as_int16(cfg_t cfg, int16_t dft);
uint16_t cfg_as_uint16(cfg_t cfg, uint16_t dft);
int32_t cfg_as_int32(cfg_t cfg, int32_t dft);
uint32_t cfg_as_uint32(cfg_t cfg, uint32_t dft);
int64_t cfg_as_int64(cfg_t cfg, int64_t dft);
uint64_t cfg_as_uint64(cfg_t cfg, uint64_t dft);
const char * cfg_as_string(cfg_t cfg, const char * dft);

/*get data by path operation*/
int8_t cfg_get_int8(cfg_t cfg, const char * path, int8_t dft);
uint8_t cfg_get_uint8(cfg_t cfg, const char * path, uint8_t dft);
int16_t cfg_get_int16(cfg_t cfg, const char * path, int16_t dft);
uint16_t cfg_get_uint16(cfg_t cfg, const char * path, uint16_t dft);
int32_t cfg_get_int32(cfg_t cfg, const char * path, int32_t dft);
uint32_t cfg_get_uint32(cfg_t cfg, const char * path, uint32_t dft);
int64_t cfg_get_int64(cfg_t cfg, const char * path, int64_t dft);
uint64_t cfg_get_uint64(cfg_t cfg, const char * path, uint64_t dft);
const char * cfg_get_string(cfg_t cfg, const char * path, const char * dft);

/*struct operations*/
cfg_t cfg_struct_find_cfg(cfg_t cfg, const char * name);

/*sequence operations*/
int cfg_seq_count(cfg_t cfg);
cfg_t cfg_seq_at(cfg_t cfg, int pos);

#ifdef __cplusplus
}
#endif

#endif


