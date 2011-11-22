#ifndef GD_TL_MANAGE_H
#define GD_TL_MANAGE_H
#include "cpe/utils/memory.h"
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_time_line_t gd_time_line_create(mem_allocrator_t alloc);
void gd_time_line_free(gd_time_line_t tl);

void gd_time_line_tick(gd_time_line_t tl);

#ifdef __cplusplus
}
#endif

#endif
