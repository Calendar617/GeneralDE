#ifndef CPE_CFG_READ_H
#define CPE_CFG_READ_H
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*basic operations*/
const char * gd_cfg_name(gd_cfg_t cfg);
void * gd_cfg_data(gd_cfg_t cfg);
int gd_cfg_type(gd_cfg_t cfg);
gd_cfg_t gd_cfg_parent(gd_cfg_t cfg);
int gd_cfg_is_value(gd_cfg_t cfg);
gd_cfg_t gd_cfg_find_cfg(gd_cfg_t cfg, const char * path);

/*get data operation*/
int8_t gd_cfg_get_int8(gd_cfg_t cfg, const char * path, int8_t dft);
uint8_t gd_cfg_get_uint8(gd_cfg_t cfg, const char * path, uint8_t dft);
int16_t gd_cfg_get_int16(gd_cfg_t cfg, const char * path, int16_t dft);
uint16_t gd_cfg_get_uint16(gd_cfg_t cfg, const char * path, uint16_t dft);
int32_t gd_cfg_get_int32(gd_cfg_t cfg, const char * path, int32_t dft);
uint32_t gd_cfg_get_uint32(gd_cfg_t cfg, const char * path, uint32_t dft);
int64_t gd_cfg_get_int64(gd_cfg_t cfg, const char * path, int64_t dft);
uint64_t gd_cfg_get_uint64(gd_cfg_t cfg, const char * path, uint64_t dft);
const char * gd_cfg_get_string(gd_cfg_t cfg, const char * path, const char * dft);

/*struct operations*/
gd_cfg_t gd_cfg_struct_find_cfg(gd_cfg_t cfg, const char * name);

/*sequence operations*/
int gd_cfg_seq_count(gd_cfg_t cfg);
gd_cfg_t gd_cfg_seq_at(gd_cfg_t cfg, int pos);

#ifdef __cplusplus
}
#endif

#endif


