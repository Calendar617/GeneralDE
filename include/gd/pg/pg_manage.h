#ifndef GD_PG_MANAGE_H
#define GD_PG_MANAGE_H
#include "cpe/utils/memory.h"
#include "pg_types.h"

gd_pg_mgr_t gd_pg_mgr_create(mem_allocrator_t alloc, gd_pg_backend_t source, size_t pageSize);
void gd_pg_mgr_free(gd_pg_mgr_t mgr);

#endif


