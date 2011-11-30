#ifndef GD_CFG_READ_H
#define GD_CFG_READ_H
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

char * gd_cfg_name(gd_cfg_t cfg);
void * gd_cfg_data(gd_cfg_t cfg);
int gd_cfg_type(gd_cfg_t cfg);
gd_cfg_t gd_cfg_parent(gd_cfg_t cfg);
int gd_cfg_is_value(gd_cfg_t cfg);

int32_t gd_cfg_get_int32(gd_cfg_t cfg, const char * path, int32_t dft);
const char * gd_cfg_get_string(gd_cfg_t cfg, const char * path, const char * dft);
gd_cfg_t gd_cfg_find_cfg(gd_cfg_t cfg, const char * path);

#ifdef __cplusplus
}
#endif

#endif


