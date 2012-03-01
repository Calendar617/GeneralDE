#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_executor_t
logic_executor_basic_create(
    logic_manage_t mgr,
    const char * name,
    logic_op_fun_t op, void * op_ctx, cfg_t args)
{
    struct logic_executor_basic * executor;
    cfg_t executor_args;

    executor_args = NULL;
    if (args) {
        if (cfg_type(args) != CPE_CFG_TYPE_STRUCT) return NULL;
        executor_args = cfg_create(mgr->m_alloc);
        if (executor_args == NULL) return NULL;

        if (cfg_merge(executor_args, args, cfg_replace, NULL) != 0) {
            cfg_free(executor_args);
            return NULL;
        }
    }

    executor = (struct logic_executor_basic *)mem_alloc(mgr->m_alloc, sizeof(struct logic_executor_basic));
    if (executor == NULL) {
        if (executor_args) cfg_free(executor_args);
        return NULL;
    }

    executor->m_mgr = mgr;
    executor->m_type = logic_executor_type_basic;
    executor->m_name = name;
    executor->m_op = op;
    executor->m_ctx = op_ctx;
    executor->m_args = executor_args;

    return (logic_executor_t)executor;
}

logic_executor_t
logic_executor_group_create(logic_manage_t mgr, const char * name) {
    struct logic_executor_group * executor;

    executor = (struct logic_executor_group *)mem_alloc(mgr->m_alloc, sizeof(struct logic_executor_group));
    if (executor == NULL) return NULL;

    executor->m_mgr = mgr;
    executor->m_type = logic_executor_type_group;
    executor->m_name = name;
    TAILQ_INIT(&executor->m_members);

    return (logic_executor_t)executor;
}

int logic_executor_group_add(logic_executor_t input_group, logic_executor_t member) {
    struct logic_executor_group * group;

    assert(input_group);
    assert(member);

    if (input_group->m_type != logic_executor_type_group) return -1;

    group = (struct logic_executor_group *)input_group;

    TAILQ_INSERT_TAIL(&group->m_members, member, m_next);

    return 0;
}

logic_executor_t
logic_executor_decorate_create(
    logic_manage_t mgr,
    const char * name,
    logic_decorate_fun_t op, void * op_ctx, logic_executor_t inner)
{
    return NULL;
}

int logic_executor_decorate_exec_inner(logic_executor_t input_decorator, logic_context_t ctx) {
    struct logic_executor_decorate * decorator;
    int32_t cur_stack_pos;

    if (input_decorator->m_type != logic_executor_type_decorate) return -1;

    decorator = (struct logic_executor_decorate *)input_decorator;

    cur_stack_pos = ctx->m_stack.m_item_pos;
    logic_stack_push(&ctx->m_stack, ctx, decorator->m_inner);

    logic_stack_exec(&ctx->m_stack, cur_stack_pos, ctx);

    return 0;
}

void logic_executor_free(logic_executor_t executor) {
    if (executor == NULL) return;

    switch(executor->m_type) {
    case logic_executor_type_basic: {
        struct logic_executor_basic * basic = (struct logic_executor_basic *)executor;
        if (basic->m_args) cfg_free(basic->m_args);
        mem_free(basic->m_mgr->m_alloc, basic);
        break;
    }
    case logic_executor_type_group: {
        logic_executor_t member;
        struct logic_executor_group * group = (struct logic_executor_group *)executor;

        TAILQ_FOREACH(member, &group->m_members, m_next) {
            logic_executor_free(member);
        }

        mem_free(group->m_mgr->m_alloc, group);
        break;
    }
    case logic_executor_type_decorate: {
        struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)executor;
        logic_executor_free(decorator->m_inner);
        mem_free(decorator->m_mgr->m_alloc, decorator);
        break;
    }
    }
}

const char * logic_executor_name(logic_executor_t executor) {
    return executor->m_name;
}

void logic_executor_dump(logic_executor_t executor, write_stream_t stream, int level) {
    if (executor == NULL) return;

    switch(executor->m_type) {
    case logic_executor_type_basic: {
        struct logic_executor_basic * basic = (struct logic_executor_basic *)executor;
        stream_putc_count(stream, ' ', level << 2);
        if (basic->m_args) {
            stream_printf(stream, "%s: ", basic->m_name);
            cfg_dump_inline(basic->m_args, stream);
        }
        else {
            stream_printf(stream, "%s", basic->m_name);
        }
        break;
    }
    case logic_executor_type_group: {
        logic_executor_t member;
        struct logic_executor_group * group = (struct logic_executor_group *)executor;
        stream_putc_count(stream, ' ', level << 2);
        stream_printf(stream, "%s:", group->m_name);

        TAILQ_FOREACH(member, &group->m_members, m_next) {
            stream_putc(stream, '\n');
            logic_executor_dump(member, stream, level + 1);
        }

        break;
    }
    case logic_executor_type_decorate: {
        struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)executor;
        stream_putc_count(stream, ' ', level << 2);
        stream_printf(stream, "%s: ", decorator->m_name);
        stream_putc(stream, '\n');
        logic_executor_dump(decorator->m_inner, stream, level + 1);
        break;
    }
    }
}
