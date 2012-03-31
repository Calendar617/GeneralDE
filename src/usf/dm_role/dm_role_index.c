#include <assert.h>
#include "cpe/dr/dr_data.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "usf/dm_role/dm_role.h"
#include "dm_role_internal_ops.h"

static uint32_t dm_role_hash_fun(struct dm_role * obj, struct dm_role_index * index) {
    return dr_entry_hash(dm_role_data(obj), index->m_entry);
}

static int dm_role_hash_cmp(struct dm_role * obj_l, struct dm_role * obj_r, struct dm_role_index * index) {
    return dr_entry_cmp(dm_role_data(obj_l), dm_role_data(obj_r) , index->m_entry) == 0;
}

struct dm_role_index *
dm_role_index_create(dm_role_manage_t mgr, const char * name, int is_unique, LPDRMETAENTRY entry) {
    struct dm_role_index * index;
    char * buf;
    size_t name_len;

    name_len = strlen(name) + 1;
    buf = mem_alloc(mgr->m_alloc, sizeof(struct dm_role_index) + name_len);
    if (buf == NULL) return NULL;

    memcpy(buf, name, name_len);

    index = (struct dm_role_index *)(buf + name_len);

    index->m_name = buf;
    index->m_id = cpe_hash_table_count(&mgr->m_indexes);
    index->m_entry = entry;
    index->m_insert_fun = is_unique ? cpe_hash_table_insert_unique : cpe_hash_table_insert;

    if (cpe_hash_table_init(
            &index->m_roles,
            mgr->m_alloc,
            (cpe_hash_fun_t) dm_role_hash_fun,
            (cpe_hash_cmp_t) dm_role_hash_cmp,
            - (sizeof(struct cpe_hash_entry) * (index->m_id + 1)),
            -1) != 0)
    {
        mem_free(mgr->m_alloc, buf);
        return NULL;
    }

    cpe_hash_entry_init(&index->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_indexes, index) != 0) {
        cpe_hash_table_fini(&index->m_roles);
        mem_free(mgr->m_alloc, buf);
        return NULL;
    }

    return index;
}

void dm_role_index_free(dm_role_manage_t mgr, struct dm_role_index * index) {
    cpe_hash_table_remove_by_ins(&mgr->m_indexes, index);
    cpe_hash_table_fini(&index->m_roles);
    mem_free(mgr->m_alloc, index->m_name);
}

int dm_role_index_add(struct dm_role_index * index,  dm_role_t role) {
    return index->m_insert_fun(&index->m_roles, role);
}

void dm_role_index_remove(struct dm_role_index * index,  dm_role_t role) {
    cpe_hash_table_remove_by_ins(&index->m_roles, role);
}

uint32_t dm_role_index_hash(const struct dm_role_index * idx) {
    return cpe_hash_str(idx->m_name, strlen(idx->m_name));
}

int dm_role_index_cmp(const struct dm_role_index * l, const struct dm_role_index * r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void dm_role_index_free_all(dm_role_manage_t mgr) {
    struct cpe_hash_it index_it;
    struct dm_role_index * index;

    cpe_hash_it_init(&index_it, &mgr->m_indexes);

    index = cpe_hash_it_next(&index_it);
    while (index) {
        struct dm_role_index * next = cpe_hash_it_next(&index_it);
        dm_role_index_free(mgr, index);
        index = next;
    }
}

