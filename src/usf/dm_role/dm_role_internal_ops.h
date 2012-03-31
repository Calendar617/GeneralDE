#ifndef USF_DM_ROLE_INTERNAL_OPS_H
#define USF_DM_ROLE_INTERNAL_OPS_H
#include "dm_role_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dm_role_index *
dm_role_index_create(dm_role_manage_t mgr, const char * name, int is_unique, LPDRMETAENTRY entry);
void dm_role_index_free(dm_role_manage_t mgr, struct dm_role_index * index);

int dm_role_index_add(struct dm_role_index * index,  dm_role_t role);
void dm_role_index_remove(struct dm_role_index * index,  dm_role_t role);

uint32_t dm_role_index_hash(const struct dm_role_index * idx);
int dm_role_index_cmp(const struct dm_role_index * l, const struct dm_role_index * r);
void dm_role_index_free_all(dm_role_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
