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

int gd_cfg_load_from_stream(gd_cfg_t cfg, read_stream_t stream);

#ifdef __cplusplus
}
#endif

#endif


