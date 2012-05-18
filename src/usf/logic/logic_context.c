#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_cfg.h"
#include "usf/logic/logic_context.h"
#include "usf/logic/logic_require.h"
#include "usf/logic/logic_data.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_context_t
logic_context_create(logic_manage_t mgr, logic_context_id_t id, size_t capacity) {
    char * buf;
    logic_context_t context;

    buf = mem_alloc(mgr->m_alloc, sizeof(struct logic_context) + capacity);
    if (buf == NULL) return NULL;

    context = (logic_context_t)buf;

    context->m_mgr = mgr;

    TAILQ_INIT(&context->m_requires);
    cpe_hash_entry_init(&context->m_hh);

    if (id != INVALID_LOGIC_CONTEXT_ID) {
        context->m_id = id;
        if (cpe_hash_table_insert_unique(&mgr->m_contexts, context) != 0) {
            mem_free(mgr->m_alloc, buf);
            return NULL;
        }
    }
    else {
        int id_try_count;
        for(id_try_count = 0; id_try_count < 2000; ++id_try_count) {
            context->m_id = context->m_mgr->m_context_id++;
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
    context->m_state = logic_context_state_init;
    context->m_capacity = capacity;
    context->m_flags = 0;
    context->m_runing = 0;
    context->m_commit_op = NULL;
    context->m_commit_ctx = NULL;
    context->m_require_waiting_count = 0;

    logic_stack_init(&context->m_stack);
    TAILQ_INIT(&context->m_datas);

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
    while (context) {
        logic_context_t next = cpe_hash_it_next(&context_it);
        logic_context_free(context);
        context = next;
    }
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

void logic_context_errno_set(logic_context_t context, int32_t v) {
    context->m_errno = v;
}

void logic_context_set_commit(
    logic_context_t context,
    logic_context_commit_fun_t op,
    void * ctx)
{
    context->m_commit_op = op;
    context->m_commit_ctx = ctx;
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
    return logic_context_state_i(context);
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
    context->m_flags &= ~((uint32_t)flag);
}

int logic_context_flag_is_enable(logic_context_t context, logic_context_flag_t flag) {
    return context->m_flags & flag;
}

void logic_context_data_dump_to_cfg(logic_context_t context, cfg_t cfg) {
    logic_data_t data;

    TAILQ_FOREACH(data, &context->m_datas, m_next) {
        cfg_t data_node;
        data_node = cfg_struct_add_struct(cfg, dr_meta_name(data->m_meta), cfg_replace);

        dr_cfg_write(data_node, logic_data_data(data), data->m_meta, NULL);
    }
}

void logic_context_execute(logic_context_t context) {
    logic_context_state_t old_state;

    if (context->m_state != logic_context_state_idle) return;
    assert(context->m_runing == 0);

    old_state = logic_context_state_i(context);

    context->m_runing = 1;
    logic_stack_exec(&context->m_stack, -1, context);
    context->m_runing = 0;

    if (context->m_errno == 0 && context->m_stack.m_item_pos == -1) {
        context->m_state = logic_context_state_done; 
    }

    logic_context_do_state_change(context, old_state);
}

int logic_context_bind(logic_context_t context, logic_executor_t executor) {
   if (executor == NULL) return -1;
   if (context->m_state != logic_context_state_init) return -1;

   logic_stack_push(&context->m_stack, context, executor);
   context->m_state = logic_context_state_idle;
   return 0;
}

void logic_context_cancel(logic_context_t context) {
    logic_context_state_t old_state;

    old_state = logic_context_state_i(context);
    context->m_state = logic_context_state_cancel;

    logic_context_do_state_change(context, old_state);
}

void logic_context_timeout(logic_context_t context) {
    logic_context_state_t old_state;

    old_state = logic_context_state_i(context);
    context->m_state = logic_context_state_timeout;

    logic_context_do_state_change(context, old_state);
}

void logic_context_do_state_change(logic_context_t context, logic_context_state_t old_sate) {
    logic_context_state_t cur_state;

    if (context->m_runing) return;

    if (old_sate > logic_context_state_idle) return;

    cur_state = logic_context_state_i(context);
    if (cur_state < logic_context_state_idle) return;

    if (cur_state == logic_context_state_idle) {
        if (context->m_flags & logic_context_flag_execute_immediately) {
            logic_context_execute(context);
        }
    }
    else {
        if (context->m_commit_op) {
            context->m_commit_op(context, context->m_commit_ctx);
        }
    }
}