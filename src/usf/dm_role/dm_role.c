#include <assert.h>
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/dm_role/dm_role.h"
#include "usf/dm_role/dm_role_manage.h"
#include "dm_role_internal_ops.h"

dm_role_t
dm_role_create(dm_role_manage_t mgr, void * data, size_t data_size, const char ** duplicate_index) {
    char * buf;
    dm_role_t role;
    size_t data_capacity;
    dm_role_id_t role_id;
    size_t index_count;
    struct cpe_hash_it index_it;
    struct dm_role_index * index;
    size_t i;

    index_count = cpe_hash_table_count(&mgr->m_indexes);

    if (duplicate_index) *duplicate_index = NULL;

    if (mgr->m_role_meta == NULL) {
        CPE_ERROR(
            mgr->m_em, "%s: dm_role_create: role meta not exist!",
            dm_role_manage_name(mgr));
        return NULL;
    }

    if (mgr->m_id_index == NULL) {
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

    role_id = dr_entry_read_int64(data, mgr->m_id_index->m_entry);
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
                dm_role_manage_name(mgr), dr_entry_name(index->m_entry));

            if (duplicate_index) *duplicate_index = dm_role_index_name(index);

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
    assert(dm_role->m_mgr->m_id_index);

    return dr_entry_read_int64(
        dm_role + 1,
        dm_role->m_mgr->m_id_index->m_entry);
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

dm_role_t dm_role_find_by_id(dm_role_manage_t mgr, dm_role_id_t id) {
    dm_role_t key;

    if (mgr->m_id_index == NULL) return NULL;

    key = dm_role_manage_key_buf(mgr);

    if (dr_entry_set_from_uint64(
            dm_role_data(key), id, mgr->m_id_index->m_entry, NULL) != 0) 
        return NULL;

    return (dm_role_t)cpe_hash_table_find(&mgr->m_id_index->m_roles, key);
}

#define DEF_DM_ROLE_FIND_BY_INDEX_FUN(__type_name, __type)              \
    dm_role_t dm_role_find_by_index_ ## __type_name(                    \
        dm_role_manage_t mgr, const char * idx_name, __type input)      \
    {                                                                   \
        struct dm_role_index index_key;                                 \
        struct dm_role_index * index;                                   \
        dm_role_t key;                                                  \
                                                                        \
        index_key.m_name = idx_name;                                    \
        index = (struct dm_role_index *)                                \
            cpe_hash_table_find(&mgr->m_indexes, &index_key);           \
        if (index == NULL) return NULL;                                 \
                                                                        \
        key = dm_role_manage_key_buf(mgr);                              \
        if (dr_entry_set_from_ ## __type_name(                          \
                dm_role_data(key), input, index->m_entry, NULL) != 0)   \
        {                                                               \
            return NULL;                                                \
        }                                                               \
                                                                        \
        return (dm_role_t)cpe_hash_table_find(&index->m_roles, key);    \
    }

DEF_DM_ROLE_FIND_BY_INDEX_FUN(int8, int8_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(uint8, uint8_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(int16, int16_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(uint16, uint16_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(int32, int32_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(uint32, uint32_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(int64, int64_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(uint64, uint64_t)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(float, float)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(double, double)
DEF_DM_ROLE_FIND_BY_INDEX_FUN(string, const char *)

dm_role_t
dm_role_find_by_index_ctype(dm_role_manage_t mgr, const char * idx_name, const void * input, int input_type) {
    struct dm_role_index index_key;
    struct dm_role_index * index;
    dm_role_t key;

    index_key.m_name = idx_name;
    index = (struct dm_role_index *)
        cpe_hash_table_find(&mgr->m_indexes, &index_key);
    if (index == NULL) return NULL;
    key = dm_role_manage_key_buf(mgr);
    if (dr_entry_set_from_ctype(
            dm_role_data(key), input, input_type, index->m_entry, NULL) != 0)
    {
        return NULL;
    }
    return (dm_role_t)cpe_hash_table_find(&index->m_roles, key);
}

