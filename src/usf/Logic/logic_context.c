#include <assert.h>
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_require.h"
#include "logic_internal_ops.h"

logic_context_t
logic_context_create(logic_manage_t mgr, logic_require_id_t id) {
    char * buf;
    logic_context_t context;

    buf = mem_alloc(mgr->m_alloc, sizeof(struct logic_context));
    if (buf == NULL) return NULL;

    context = (logic_context_t)buf;

    context->m_id = id == INVALID_LOGIC_CONTEXT_ID ? ++mgr->m_context_id : id;
    context->m_mgr = mgr;
    
    TAILQ_INIT(&context->m_requires);
    cpe_hash_entry_init(&context->m_hh);

    if (cpe_hash_table_insert_unique(&mgr->m_contexts, context) != 0) {
        mem_free(mgr->m_alloc, buf);
        return NULL;
    }

    return context;
}

void logic_context_free(logic_context_t context) {
    assert(context);

    while(!TAILQ_EMPTY(&context->m_requires)) {
        logic_require_free(TAILQ_FIRST(&context->m_requires));
    }

    cpe_hash_table_remove_by_ins(&context->m_mgr->m_contexts, context);

    mem_free(context->m_mgr->m_alloc, context);
}

logic_context_id_t
logic_context_id(logic_context_t context) {
    return context->m_id;
}

logic_context_state_t
logic_context_state(logic_context_t context) {
    logic_context_state_t state;
    logic_require_t require;

    assert(context);

    state = logic_context_idle;

    TAILQ_FOREACH(require, &context->m_requires, m_next) {
        switch (require->m_state) {
        case logic_require_error:
            return logic_context_error;
        case logic_require_waiting:
            state = logic_context_waiting;
            break;
        default:
            //DO NOTHING
            break;
        }
    }

    return state;
}

uint32_t logic_context_hash(const struct logic_context * context) {
    return context->m_id;
}

int logic_context_cmp(const struct logic_context * l, const struct logic_context * r) {
    return l->m_id == r->m_id;
}

