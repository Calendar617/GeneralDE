#include <assert.h>
#include "cpe/pal/pal_strings.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "usf/logic/logic_data.h"
#include "logic_internal_ops.h"

logic_data_t
logic_data_get_or_create(logic_context_t context, LPDRMETA meta, size_t capacity) {
    const char * name;
    logic_data_t old_data;
    logic_data_t new_data;

    if (capacity == 0) capacity = dr_meta_size(meta);

    name = dr_meta_name(meta);
    old_data = logic_data_find(context, name);
    if (old_data && old_data->m_capacity >= capacity) return old_data;

    new_data = (logic_data_t)mem_alloc(context->m_mgr->m_alloc, sizeof(struct logic_data) + capacity);
    if (new_data == NULL) return NULL;

    new_data->m_ctx = context;
    new_data->m_name = name;
    new_data->m_meta = meta;
    new_data->m_capacity = capacity;
    cpe_hash_entry_init(&new_data->m_hh);

    if (old_data) {
        memcpy(new_data + 1, old_data + 1, old_data->m_capacity);
        logic_data_free(old_data);
    }
    else {
        bzero(new_data + 1, capacity);
        dr_meta_set_defaults(new_data + 1, capacity, meta, 0);
    }

    if (cpe_hash_table_insert_unique(&context->m_mgr->m_datas, new_data) != 0) {
        mem_free(context->m_mgr->m_alloc, new_data);
        return NULL;
    }

    TAILQ_INSERT_TAIL(&context->m_datas, new_data, m_next);

    return new_data;
}

void logic_data_free(logic_data_t data) {
    assert(data);
    cpe_hash_table_remove_by_ins(&data->m_ctx->m_mgr->m_datas, data);
    TAILQ_REMOVE(&data->m_ctx->m_datas, data, m_next);
    mem_free(data->m_ctx->m_mgr->m_alloc, data);
}

void logic_data_free_all(logic_manage_t mgr) {
    struct cpe_hash_it data_it;
    logic_data_t data;

    cpe_hash_it_init(&data_it, &mgr->m_datas);

    data = cpe_hash_it_next(&data_it);
    while (data) {
        logic_data_t next = cpe_hash_it_next(&data_it);
        logic_data_free(data);
        data = next;
    }
}

logic_data_t logic_data_find(logic_context_t context, const char * name) {
    struct logic_data key;

    key.m_ctx = context;
    key.m_name = name;

    return (logic_data_t)cpe_hash_table_find(&context->m_mgr->m_datas, &key);
}

LPDRMETA logic_data_meta(logic_data_t data) {
    return data->m_meta;
}

void * logic_data_data(logic_data_t data) {
    return data + 1;
}

size_t logic_data_capacity(logic_data_t data) {
    return data->m_capacity;
}

const char * logic_data_name(logic_data_t data) {
    return data->m_name;
}

uint32_t logic_data_hash(const struct logic_data * data) {
    return (cpe_hash_str(data->m_name, strlen(data->m_name)) << 16)
        | (data->m_ctx->m_id & 0xFF);
}

int logic_data_cmp(const struct logic_data * l, const struct logic_data * r) {
    return l->m_ctx->m_id == r->m_ctx->m_id
        && strcmp(l->m_name, r->m_name) == 0;
}
