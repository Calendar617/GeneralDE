#ifndef GD_TL_MANAGE_H
#define GD_TL_MANAGE_H
#include "cpe/utils/memory.h"
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_tl_manage_t gd_tl_manage_create(mem_allocrator_t alloc);
void gd_tl_manage_free(gd_tl_manage_t tm);
void gd_tl_manage_tick(gd_tl_manage_t tm);

gd_tl_t gd_tl_create(gd_tl_manage_t tm);
void gd_tl_set_opt(gd_tl_t tl, ...);

#ifdef __cplusplus
}
#endif

#endif
