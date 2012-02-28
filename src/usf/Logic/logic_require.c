#include <assert.h>
#include "usf/logic/logic_require.h"
#include "usf/logic/logic_require_type.h"
#include "logic_internal_ops.h"

logic_require_t
logic_require_create(logic_context_t context, cpe_hash_string_t require_name) {
    logic_require_type_t require_type;
    logic_require_t require;
    int id_try_count;

    require_type = logic_require_type_find(context->m_mgr, require_name);
    if (require_type == NULL) return NULL;

    require = mem_alloc(context->m_mgr->m_alloc, sizeof(struct logic_require));
    if (require == NULL) return NULL;

    require->m_ctx = context;
    require->m_state = logic_require_waiting;
    require->m_type = require_type;
    
    cpe_hash_entry_init(&context->m_hh);
    for(id_try_count = 0; id_try_count < 2000; ++id_try_count) {
        require->m_id = ++context->m_mgr->m_require_id;
        if (cpe_hash_table_insert_unique(&context->m_mgr->m_requires, require) == 0) {
            break;
        }
    }

    if (id_try_count >= 2000) {
        mem_free(context->m_mgr->m_alloc, require);
        return NULL;
    }
    
    TAILQ_INSERT_TAIL(&context->m_requires, require, m_next);

    return require;
}

void logic_require_free(logic_require_t require) {
    assert(require);

    TAILQ_REMOVE(&require->m_ctx->m_requires, require, m_next);
    cpe_hash_table_remove_by_ins(&require->m_ctx->m_mgr->m_requires, require);
    mem_free(require->m_ctx->m_mgr->m_alloc, require);
}

void logic_require_free_all(logic_manage_t mgr) {
    struct cpe_hash_it require_it;
    logic_require_t require;

    cpe_hash_it_init(&require_it, &mgr->m_requires);

    require = cpe_hash_it_next(&require_it);
    do {
        logic_require_t next = cpe_hash_it_next(&require_it);
        logic_require_free(require);
        require = next;
    } while(require);
}

logic_require_t
logic_require_find(logic_manage_t mgr, logic_require_id_t id) {
    struct logic_require key;

    key.m_id = id;
    return (logic_require_t)cpe_hash_table_find(&mgr->m_requires, &key);
}

logic_require_state_t
logic_require_state(logic_require_t require) {
    return require->m_state;
}

void logic_require_set_done(logic_require_t require) {
    /* logic_context_state_t state; */
    /* logic_require_t require; */

    /* assert(context); */

    /* state = logic_context_idle; */

    /* TAILQ_FOREACH(require, &context->m_requires, m_next) { */
    /*     switch (require->m_state) { */
    /*     case logic_require_error: */
    /*         return logic_context_error; */
    /*     case logic_require_waiting: */
    /*         state = logic_context_waiting; */
    /*         break; */
    /*     default: */
    /*         //DO NOTHING */
    /*         break; */
    /*     } */
    /* } */

    /* return state; */
}

void logic_require_set_error(logic_require_t require) {
}

uint32_t logic_require_hash(const struct logic_require * require) {
    return require->m_id;
}

int logic_require_cmp(const struct logic_require * l, const struct logic_require * r) {
    return l->m_id == r->m_id;
}
