#include <assert.h>
#include "usf/logic/logic_manage.h"
#include "logic_internal_ops.h"

logic_manage_t
logic_manage_create(mem_allocrator_t alloc) {
    char * buf;
    logic_manage_t mgr;

    buf = mem_alloc(alloc, sizeof(struct logic_manage));
    if (buf == NULL) return NULL;

    mgr = (logic_manage_t)buf;
    mgr->m_alloc = alloc;

    if (cpe_hash_table_init(
            &mgr->m_require_types,
            alloc,
            (cpe_hash_fun_t) logic_require_type_hash,
            (cpe_hash_cmp_t) logic_require_type_cmp,
            CPE_HASH_OBJ2ENTRY(logic_require_type, m_hh),
            -1) != 0)
    {
        mem_free(alloc, mgr);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_requires,
            alloc,
            (cpe_hash_fun_t) logic_require_hash,
            (cpe_hash_cmp_t) logic_require_cmp,
            CPE_HASH_OBJ2ENTRY(logic_require, m_hh),
            -1) != 0)
    {
        mem_free(alloc, mgr);
        cpe_hash_table_fini(&mgr->m_require_types);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_datas,
            alloc,
            (cpe_hash_fun_t) logic_data_hash,
            (cpe_hash_cmp_t) logic_data_cmp,
            CPE_HASH_OBJ2ENTRY(logic_data, m_hh),
            -1) != 0)
    {
        mem_free(alloc, mgr);
        cpe_hash_table_fini(&mgr->m_require_types);
        cpe_hash_table_fini(&mgr->m_requires);
        return NULL;
    }

    if (cpe_hash_table_init(
            &mgr->m_contexts,
            alloc,
            (cpe_hash_fun_t) logic_context_hash,
            (cpe_hash_cmp_t) logic_context_cmp,
            CPE_HASH_OBJ2ENTRY(logic_context, m_hh),
            -1) != 0)
    {
        mem_free(alloc, mgr);
        cpe_hash_table_fini(&mgr->m_require_types);
        cpe_hash_table_fini(&mgr->m_requires);
        cpe_hash_table_fini(&mgr->m_datas);
        return NULL;
    }

    return mgr;
}

void logic_manage_free(logic_manage_t mgr) {
    assert(mgr);

    logic_require_free_all(mgr);
    logic_data_free_all(mgr);
    logic_context_free_all(mgr);
    logic_require_type_free_all(mgr);

    cpe_hash_table_fini(&mgr->m_contexts);
    cpe_hash_table_fini(&mgr->m_requires);
    cpe_hash_table_fini(&mgr->m_datas);
    cpe_hash_table_fini(&mgr->m_require_types);

    mem_free(mgr->m_alloc, mgr);
}
