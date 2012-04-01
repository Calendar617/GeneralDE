#ifndef USF_DM_ROLE_INTERNAL_TYPES_H
#define USF_DM_ROLE_INTERNAL_TYPES_H
#include "cpe/utils/hash.h"
#include "gd/dr_store/dr_store_types.h"
#include "usf/dm_role/dm_role_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dm_role_index;

struct dm_role_manage {
    gd_app_context_t m_app;
    mem_allocrator_t m_alloc;
    error_monitor_t m_em;

    int m_debug;

    dr_ref_t m_metalib;
    LPDRMETA m_role_meta;

    dm_role_t m_key_buf;
    struct dm_role_index * m_id_index;
    struct cpe_hash_table m_indexes;
};

struct dm_role {
    dm_role_manage_t m_mgr; 
};

struct dm_role_index {
    int m_id;
    const char * m_name;
    LPDRMETAENTRY m_entry;
    struct cpe_hash_table m_roles;
    int (*m_insert_fun)(cpe_hash_table_t hstable, void * obj);

    struct cpe_hash_entry m_hh;
};

#ifdef __cplusplus
}
#endif

#endif
