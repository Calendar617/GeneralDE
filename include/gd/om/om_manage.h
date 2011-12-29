#ifndef GD_OM_MANAGE_H
#define GD_OM_MANAGE_H
#include "cpe/utils/memory.h"
#include "om_types.h"

gd_om_mgr_t gd_om_mgr_create(mem_allocrator_t alloc);
void gd_om_mgr_free(gd_om_mgr_t mgr);

#endif


