#ifndef USF_DM_ROLE_H
#define USF_DM_ROLE_H
#include "dm_role_types.h"

#ifdef __cplusplus
extern "C" {
#endif

dm_role_t
dm_role_create(dm_role_manage_t mgr, void * data, size_t data_size);

void dm_role_free(dm_role_t dm_role);

dm_role_id_t dm_role_id(dm_role_t dm_role);
void * dm_role_data(dm_role_t dm_role);
size_t dm_role_data_capacity(dm_role_t dm_role);
LPDRMETA dm_role_meta(dm_role_t dm_role);

#ifdef __cplusplus
}
#endif

#endif
