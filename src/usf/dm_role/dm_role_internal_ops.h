#ifndef USF_DM_ROLE_INTERNAL_OPS_H
#define USF_DM_ROLE_INTERNAL_OPS_H
#include "dm_role_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*role manage ops*/
#define dm_role_manage_is_empty(mgr) \
    (mgr->m_id_index && cpe_hash_table_count(&mgr->m_id_index->m_roles) > 0 ? 0 : 1)

dm_role_t dm_role_manage_key_buf(dm_role_manage_t mgr);

/*role index ops*/
struct dm_role_index *
dm_role_index_create(dm_role_manage_t mgr, LPDRMETAENTRY entry, int is_unique);
void dm_role_index_free(dm_role_manage_t mgr, struct dm_role_index * index);

int dm_role_index_add(struct dm_role_index * index,  dm_role_t role);
void dm_role_index_remove(struct dm_role_index * index,  dm_role_t role);

const char * dm_role_index_name(struct dm_role_index * index);

uint32_t dm_role_index_hash(const struct dm_role_index * idx);
int dm_role_index_cmp(const struct dm_role_index * l, const struct dm_role_index * r);
void dm_role_index_free_all(dm_role_manage_t mgr);

#ifdef __cplusplus
}
#endif

#endif
