#include <assert.h>
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_require.h"
#include "usf/logic/logic_data.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_context_t
logic_context_create(logic_manage_t mgr, logic_require_id_t id, logic_executor_t executor, size_t capacity) {
    char * buf;
    logic_context_t context;

    if (executor == NULL) return NULL;

    buf = mem_alloc(mgr->m_alloc, sizeof(struct logic_context) + capacity);
    if (buf == NULL) return NULL;

    context = (logic_context_t)buf;

    context->m_id = id == INVALID_LOGIC_CONTEXT_ID ? ++mgr->m_context_id : id;
    context->m_mgr = mgr;

    TAILQ_INIT(&context->m_requires);
    cpe_hash_entry_init(&context->m_hh);

    if (id == INVALID_LOGIC_CONTEXT_ID) {
        if (cpe_hash_table_insert_unique(&mgr->m_contexts, context) != 0) {
            mem_free(mgr->m_alloc, buf);
            return NULL;
        }
    }
    else {
        int id_try_count;
        for(id_try_count = 0; id_try_count < 2000; ++id_try_count) {
            context->m_id = ++context->m_mgr->m_context_id;
            if (cpe_hash_table_insert_unique(&mgr->m_contexts, context) == 0) {
                break;
            }
        }

        if (id_try_count >= 2000) {
            mem_free(mgr->m_alloc, buf);
            return NULL;
        }
    }

    context->m_errno = 0;
    context->m_state = logic_context_idle;
    context->m_capacity = capacity;
    context->m_flags = 0;
    logic_stack_init(&context->m_stack);
    logic_stack_push(&context->m_stack, context, executor);

    return context;
}

void logic_context_free(logic_context_t context) {
    assert(context);

    while(!TAILQ_EMPTY(&context->m_requires)) {
        logic_require_free(TAILQ_FIRST(&context->m_requires));
    }

    while(!TAILQ_EMPTY(&context->m_datas)) {
        logic_data_free(TAILQ_FIRST(&context->m_datas));
    }

    logic_stack_fini(&context->m_stack, context);

    cpe_hash_table_remove_by_ins(&context->m_mgr->m_contexts, context);

    mem_free(context->m_mgr->m_alloc, context);
}

void logic_context_free_all(logic_manage_t mgr) {
    struct cpe_hash_it context_it;
    logic_context_t context;

    cpe_hash_it_init(&context_it, &mgr->m_contexts);

    context = cpe_hash_it_next(&context_it);
    do {
        logic_context_t next = cpe_hash_it_next(&context_it);
        logic_context_free(context);
        context = next;
    } while(context);
}

logic_context_t
logic_context_find(logic_manage_t mgr, logic_context_id_t id) {
    struct logic_context key;

    key.m_id = id;
    return (logic_context_t)cpe_hash_table_find(&mgr->m_contexts, &key);
}

logic_context_id_t
logic_context_id(logic_context_t context) {
    return context->m_id;
}

int32_t logic_context_errno(logic_context_t context) {
    return context->m_errno;
}

logic_manage_t
logic_context_mgr(logic_context_t context) {
    return context->m_mgr;
}

gd_app_context_t
logic_context_app(logic_context_t context) {
    return context->m_mgr->m_app;
}

logic_context_state_t
logic_context_state(logic_context_t context) {
    return context->m_state;
}

size_t logic_context_capacity(logic_context_t context) {
    return context->m_capacity;
}

void * logic_context_data(logic_context_t context) {
    return context + 1;
}

uint32_t logic_context_hash(const struct logic_context * context) {
    return context->m_id;
}

int logic_context_cmp(const struct logic_context * l, const struct logic_context * r) {
    return l->m_id == r->m_id;
}

void logic_context_execute(logic_context_t context) {
    logic_stack_exec(&context->m_stack, -1, context);
}

uint32_t logic_context_flags(logic_context_t context) {
    return context->m_flags;
}

void logic_context_flags_set(logic_context_t context, uint32_t flag) {
    context->m_flags = flag;
}

void logic_context_flag_enable(logic_context_t context, logic_context_flag_t flag) {
    context->m_flags |= flag;
}

void logic_context_flag_disable(logic_context_t context, logic_context_flag_t flag) {
    context->m_flags &= ~flag;
}

int logic_context_flag_is_enable(logic_context_t context, logic_context_flag_t flag) {
    return context->m_flags & flag;
}
