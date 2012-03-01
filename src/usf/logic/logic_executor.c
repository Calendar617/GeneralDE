#include <assert.h>
#include "cpe/cfg/cfg_manage.h"
#include "cpe/cfg/cfg_read.h"
#include "usf/logic/logic_executor.h"
#include "logic_internal_ops.h"

logic_executor_t
logic_executor_basic_create(logic_manage_t mgr, logic_op_fun_t op, void * op_ctx, cfg_t args) {
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
    executor->m_op = op;
    executor->m_ctx = op_ctx;
    executor->m_args = executor_args;

    return (logic_executor_t)executor;
}

logic_executor_t
logic_executor_group_create(logic_manage_t mgr) {
    struct logic_executor_group * executor;

    executor = (struct logic_executor_group *)mem_alloc(mgr->m_alloc, sizeof(struct logic_executor_group));
    if (executor == NULL) return NULL;

    executor->m_mgr = mgr;
    executor->m_type = logic_executor_type_group;
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
logic_executor_decorate_create(logic_manage_t mgr, logic_decorate_fun_t op, void * op_ctx, logic_executor_t inner) {
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
    assert(executor);
REENTER:

    switch(executor->m_type) {
    case logic_executor_type_basic: {
        struct logic_executor_basic * basic = (struct logic_executor_basic *)executor;
        if (basic->m_args) cfg_free(basic->m_args);
        mem_free(basic->m_mgr->m_alloc, basic);
        break;
    }
    case logic_executor_type_group: {
        struct logic_executor_group * group = (struct logic_executor_group *)executor;

        if (!TAILQ_EMPTY(&group->m_members)) {
            logic_executor_t member = TAILQ_FIRST(&group->m_members);
            while(!TAILQ_EMPTY(&group->m_members)) {
                logic_executor_free(member);
                member = TAILQ_FIRST(&group->m_members);
            }

            mem_free(group->m_mgr->m_alloc, group);
            executor = member;
            goto REENTER;
        }
        else {
            mem_free(group->m_mgr->m_alloc, group);
        }

        break;
    }
    case logic_executor_type_decorate: {
        struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)executor;
        executor = decorator->m_inner;
        mem_free(decorator->m_mgr->m_alloc, decorator);
        goto REENTER;
    }
    }
}
