#ifndef GD_CFG_MANAGE_H
#define GD_CFG_MANAGE_H
#include "cpe/utils/memory.h"
#include "cfg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_cfg_t gd_cfg_create(mem_allocrator_t alloc);
void gd_cfg_free(gd_cfg_t cfg);

#ifdef __cplusplus
}
#endif

#endif


