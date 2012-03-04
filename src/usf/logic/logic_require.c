#include <assert.h>
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_require.h"
#include "usf/logic/logic_require_type.h"
#include "logic_internal_ops.h"

static void logic_require_do_cancel(logic_require_t require);

logic_require_t
logic_require_create(logic_context_t context, cpe_hash_string_t require_name, size_t capacity) {
    logic_require_type_t require_type;
    logic_require_t require;
    int id_try_count;

    require_type = logic_require_type_find(context->m_mgr, require_name);
    if (require_type == NULL) return NULL;

    require = mem_alloc(context->m_mgr->m_alloc, sizeof(struct logic_require) + capacity);
    if (require == NULL) return NULL;

    require->m_ctx = context;
    require->m_state = logic_require_state_waiting;
    require->m_type = require_type;
    require->m_capacity = capacity;

    cpe_hash_entry_init(&require->m_hh);
    for(id_try_count = 0; id_try_count < 2000; ++id_try_count) {
        require->m_id = context->m_mgr->m_require_id++;
        if (cpe_hash_table_insert_unique(&context->m_mgr->m_requires, require) == 0) {
            break;
        }
    }

    if (id_try_count >= 2000) {
        mem_free(context->m_mgr->m_alloc, require);
        return NULL;
    }
    
    TAILQ_INSERT_TAIL(&context->m_requires, require, m_next);
    ++context->m_require_waiting_count;

    return require;
}

void logic_require_free(logic_require_t require) {
    assert(require);

    if (require->m_state == logic_require_state_waiting) {
        logic_require_do_cancel(require);
    }

    if (require->m_type->m_destory_op) {
        require->m_type->m_destory_op(require, require->m_type->m_destory_ctx);
    }

    TAILQ_REMOVE(&require->m_ctx->m_requires, require, m_next);
    cpe_hash_table_remove_by_ins(&require->m_ctx->m_mgr->m_requires, require);
    mem_free(require->m_ctx->m_mgr->m_alloc, require);
}

void logic_require_free_all(logic_manage_t mgr) {
    struct cpe_hash_it require_it;
    logic_require_t require;

    cpe_hash_it_init(&require_it, &mgr->m_requires);

    require = cpe_hash_it_next(&require_it);
    while(require) {
        logic_require_t next = cpe_hash_it_next(&require_it);
        logic_require_free(require);
        require = next;
    }
}

logic_require_t
logic_require_find(logic_manage_t mgr, logic_require_id_t id) {
    struct logic_require key;

    key.m_id = id;
    return (logic_require_t)cpe_hash_table_find(&mgr->m_requires, &key);
}

logic_require_id_t logic_require_id(logic_require_t require) {
    return require->m_id;
}

logic_require_state_t logic_require_state(logic_require_t require) {
    return require->m_state;
}

size_t logic_require_capacity(logic_require_t require) {
    return require->m_capacity;
}

void * logic_require_data(logic_require_t require) {
    return require + 1;
}

static void logic_require_do_cancel(logic_require_t require) {
    logic_context_state_t old_state;

    if (require->m_state != logic_require_state_waiting) return;

    old_state = logic_context_state_i(require->m_ctx);

    --require->m_ctx->m_require_waiting_count;
    require->m_state = logic_require_state_canceling;

    if (require->m_type->m_cancel_op) {
        require->m_type->m_cancel_op(require, require->m_type->m_cancel_ctx);
    }

    if (require->m_state == logic_require_state_canceling) {
        require->m_state = logic_require_state_canceled;
    }

    logic_context_do_state_change(require->m_ctx, old_state);
}

void logic_require_cancel(logic_require_t require) {
    logic_require_do_cancel(require);

    if (!(require->m_ctx->m_flags & logic_context_flag_require_keep)) {
        logic_require_free(require);
    }
}

void logic_require_set_done(logic_require_t require) {
    logic_context_t ctx;
    logic_context_state_t old_state;

    if (require->m_state != logic_require_state_waiting) {
        require->m_state = logic_require_state_done;
        return;
    }

    ctx = require->m_ctx;
    old_state = logic_context_state_i(ctx);

    --ctx->m_require_waiting_count;
    require->m_state = logic_require_state_done;

    if (!(require->m_ctx->m_flags & logic_context_flag_require_keep)) {
        logic_require_free(require);
    }

    logic_context_do_state_change(ctx, old_state);
}

void logic_require_set_error(logic_require_t require) {
    logic_context_t ctx;
    logic_context_state_t old_state;

    if (require->m_state != logic_require_state_waiting) {
        require->m_state = logic_require_state_error;
        return;
    }

    ctx = require->m_ctx;
    old_state = logic_context_state_i(ctx);

    --ctx->m_require_waiting_count;
    require->m_state = logic_require_state_error;

    if (!(require->m_ctx->m_flags & logic_context_flag_require_keep)) {
        logic_require_free(require);
    }

    logic_context_do_state_change(ctx, old_state);
}

uint32_t logic_require_hash(const struct logic_require * require) {
    return require->m_id;
}

int logic_require_cmp(const struct logic_require * l, const struct logic_require * r) {
    return l->m_id == r->m_id;
}
