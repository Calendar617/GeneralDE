#include <assert.h>
#include "usf/logic/logic_require.h"
#include "usf/logic/logic_require_type.h"
#include "logic_internal_ops.h"

logic_require_type_t
logic_require_type_create(logic_manage_t mgr, const char * name) {
    logic_require_type_t rt;
    size_t name_capacity;
    char * buf;

    name_capacity = cpe_hs_len_to_binary_len(strlen(name));

    buf =  (char *)mem_alloc(mgr->m_alloc, sizeof(struct logic_require_type) + name_capacity);
    if (buf == NULL) return NULL;

    cpe_hs_init((cpe_hash_string_t)buf, name_capacity, name);

    rt = (logic_require_type_t)(buf + name_capacity);
    rt->m_mgr = mgr;
    rt->m_name = (cpe_hash_string_t)buf;

    cpe_hash_entry_init(&rt->m_hh);
    if (cpe_hash_table_insert_unique(&mgr->m_require_types, rt) != 0) {
        mem_free(mgr->m_alloc, buf);
        return NULL;
    }

    return rt;
}

void logic_require_type_free(logic_require_type_t rt) {
    struct cpe_hash_it require_it;
    logic_require_t require;
    assert(rt);

    cpe_hash_it_init(&require_it, &rt->m_mgr->m_requires);

    require = cpe_hash_it_next(&require_it);
    while (require) {
        logic_require_t next = cpe_hash_it_next(&require_it);

        if (require->m_type == rt) {
            logic_require_free(require);
        }

        require = next;
    };

    cpe_hash_table_remove_by_ins(&rt->m_mgr->m_require_types, rt);
    mem_free(rt->m_mgr->m_alloc, rt->m_name);
}

void logic_require_type_free_all(logic_manage_t mgr) {
    struct cpe_hash_it require_type_it;
    logic_require_type_t require_type;

    cpe_hash_it_init(&require_type_it, &mgr->m_require_types);

    require_type = cpe_hash_it_next(&require_type_it);
    while (require_type) {
        logic_require_type_t next = cpe_hash_it_next(&require_type_it);
        logic_require_type_free(require_type);
        require_type = next;
    }
}

logic_require_type_t
logic_require_type_find(logic_manage_t mgr, cpe_hash_string_t type_name) {
    struct logic_require_type key;

    key.m_name = type_name;
    return (logic_require_type_t)cpe_hash_table_find(&mgr->m_require_types, &key);
}

uint32_t logic_require_type_hash(const struct logic_require_type * rt) {
    return cpe_hs_value(rt->m_name);
}

int logic_require_type_cmp(const struct logic_require_type * l, const struct logic_require_type * r) {
    return cpe_hs_cmp(l->m_name, r->m_name) == 0 ? 1 : 0;
}
