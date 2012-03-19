#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "usf/logic/logic_executor_type.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_executor_type_t
logic_executor_type_create(
    logic_executor_type_group_t group,
    const char * name,
    logic_executor_category_t category)
{
    logic_executor_type_t type;
    size_t name_len;
    void * buf;

    name_len = strlen(name) + 1;

    buf = mem_alloc(group->m_alloc, sizeof(struct logic_executor_type) + name_len);
    if (buf == NULL) return NULL;

    memcpy(buf, name, name_len);

    type = (logic_executor_type_t)(buf + name_len);
    type->m_group = group;
    type->m_name = (char *)buf;
    type->m_category = category;
    type->m_op = NULL;
    type->m_ctx = NULL;

    cpe_hash_entry_init(&type->m_hh);
    if (cpe_hash_table_insert_unique(&group->m_types, type) != 0) {
        mem_free(group->m_alloc, buf);
        return NULL;
    }

    return type;
}

void logic_executor_type_free(logic_executor_type_t t) {
    cpe_hash_table_remove_by_ins(&t->m_group->m_types, t);
    mem_free(t->m_group->m_alloc, (void*)t->m_name);
}

void logic_executor_type_free_all(logic_executor_type_group_t group) {
    struct cpe_hash_it type_it;
    logic_executor_type_t type;

    cpe_hash_it_init(&type_it, &group->m_types);

    type = cpe_hash_it_next(&type_it);
    while(type) {
        logic_executor_type_t next = cpe_hash_it_next(&type_it);
        logic_executor_type_free(type);
        type = next;
    }
}

logic_executor_type_t
logic_executor_type_find(
    logic_executor_type_group_t group,
    const char * name)
{
    struct logic_executor_type key;
    key.m_name = (char*)name;

    return (logic_executor_type_t)cpe_hash_table_find(&group->m_types, &key);
}

const char * logic_executor_type_name(logic_executor_type_t type) {
    return type->m_name;
}

void * logic_executor_type_ctx(logic_executor_type_t type) {
    return type->m_ctx;
}

int logic_executor_type_bind_basic(logic_executor_type_t type, logic_op_fun_t fun, void * ctx) {
    assert(type);
    if (type->m_category != logic_executor_category_basic) return -1;

    type->m_op = fun;
    type->m_ctx = ctx;

    return 0;
}

int logic_executor_type_bind_decorate(logic_executor_type_t type, logic_decorate_fun_t fun, void * ctx) {
    assert(type);
    if (type->m_category != logic_executor_category_decorate) return -1;

    type->m_op = fun;
    type->m_ctx = ctx;

    return 0;
}

uint32_t logic_executor_type_hash(const struct logic_executor_type * type) {
    return cpe_hash_str(type->m_name, strlen(type->m_name));
}

int logic_executor_type_cmp(const struct logic_executor_type * l, const struct logic_executor_type * r) {
    return strcmp(l->m_name, r->m_name) == 0;
}

void logic_executor_type_init_defaults(logic_executor_type_group_t group) {
    logic_executor_type_create(group, "group", logic_executor_category_group);

    logic_executor_type_bind_decorate(
        logic_executor_type_create(group, "protect", logic_executor_category_decorate),
        logic_executor_decorate_protect,
        NULL);
}
