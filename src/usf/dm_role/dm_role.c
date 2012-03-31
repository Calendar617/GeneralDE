#include <assert.h>
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/dm_role/dm_role.h"
#include "usf/dm_role/dm_role_manage.h"
#include "dm_role_internal_ops.h"

dm_role_t
dm_role_create(dm_role_manage_t mgr, void * data, size_t data_size) {
    char * buf;
    dm_role_t role;
    size_t data_capacity;
    dm_role_id_t role_id;
    size_t index_count;
    struct cpe_hash_it index_it;
    struct dm_role_index * index;
    size_t i;

    index_count = cpe_hash_table_count(&mgr->m_indexes);

    if (mgr->m_role_meta == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: dm_role_create: role meta not exist!",
            dm_role_manage_name(mgr));
        return NULL;
    }

    if (mgr->m_role_id_entry == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: dm_role_create: role id entry not exist!",
            dm_role_manage_name(mgr));
        return NULL;
    }

    data_capacity = dr_meta_size(mgr->m_role_meta);
    if (data_size > data_capacity) {
        CPE_ERROR(
            mgr->m_em, "%s: dm_role_create: data too long, data_size=%d, data_capacity=%d!",
            dm_role_manage_name(mgr), (int)data_size, (int)data_capacity);
        return NULL;
    }

    role_id = dr_entry_read_int64(data, role->m_mgr->m_role_id_entry);
    if (role_id == 0) {
        //TODO
    }

    buf = (char *)mem_alloc(
        mgr->m_alloc,
        sizeof(struct cpe_hash_entry) * index_count
        + sizeof(struct dm_role)
        + data_capacity);
    if (buf == NULL) return NULL;

    role = (dm_role_t)(buf + sizeof(struct cpe_hash_entry) * index_count);

    role->m_mgr = mgr;

    memcpy(dm_role_data(role), data, data_capacity);

    for(i = 0; i < index_count; ++i) {
        cpe_hash_entry_init(((struct cpe_hash_entry*)buf) + i);
    }

    cpe_hash_it_init(&index_it, &mgr->m_indexes);
    while((index = cpe_hash_it_next(&index_it))) {
        if (dm_role_index_add(index, role) != 0) {
            struct dm_role_index * index_fall_back;

            CPE_ERROR(
                mgr->m_em, "%s: dm_role_create: add to index %s: duplicate!",
                dm_role_manage_name(mgr), index->m_name);

            cpe_hash_it_init(&index_it, &mgr->m_indexes);
            while((index_fall_back = cpe_hash_it_next(&index_it)) && index_fall_back != index) {
                dm_role_index_remove(index_fall_back, role);
            }
            
            mem_free(mgr->m_alloc, buf);
            return NULL;
        }
    }

    return role;
}

void dm_role_free(dm_role_t dm_role) {
    struct cpe_hash_it index_it;
    struct dm_role_index * index;
    size_t index_count;

    index_count = cpe_hash_table_count(&dm_role->m_mgr->m_indexes);

    cpe_hash_it_init(&index_it, &dm_role->m_mgr->m_indexes);
    while((index = cpe_hash_it_next(&index_it))) {
        dm_role_index_remove(index, dm_role);
    }

    mem_free(dm_role->m_mgr->m_alloc, ((char*)dm_role) - sizeof(struct cpe_hash_entry) * index_count);
}

dm_role_id_t dm_role_id(dm_role_t dm_role) {
    assert(dm_role->m_mgr->m_role_id_entry);

    return dr_entry_read_int64(
        dm_role + 1,
        dm_role->m_mgr->m_role_id_entry);
}

void * dm_role_data(dm_role_t dm_role) {
    return dm_role + 1;
}

size_t dm_role_data_capacity(dm_role_t dm_role) {
    assert(dm_role->m_mgr->m_role_meta);
    return dr_meta_size(dm_role->m_mgr->m_role_meta);
}

LPDRMETA dm_role_meta(dm_role_t dm_role) {
    assert(dm_role->m_mgr->m_role_meta);

    return dm_role->m_mgr->m_role_meta;
}
