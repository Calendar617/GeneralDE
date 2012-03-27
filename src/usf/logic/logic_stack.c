#include <assert.h>
#include "gd/app/app_context.h"
#include "usf/logic/logic_context.h"
#include "logic_internal_ops.h"

#define LOGIC_STACK_INLINE_ITEM_COUNT \
    ( sizeof(((struct logic_stack*)0)->m_inline_items) \
      / sizeof(((struct logic_stack*)0)->m_inline_items[0]) )

void logic_stack_init(struct logic_stack * stack) {
    stack->m_item_pos = -1;
    stack->m_extern_items = NULL;
    stack->m_extern_items_capacity = 0;
}

void logic_stack_fini(struct logic_stack * stack, logic_context_t context) {
    if (stack->m_extern_items) {
        mem_free(context->m_mgr->m_alloc, stack->m_extern_items);
    }
}

void logic_stack_push(struct logic_stack * stack, logic_context_t context, logic_executor_t executor) {
    struct logic_stack_item * stack_item;

REINTER:
    if (stack->m_item_pos + 1 < LOGIC_STACK_INLINE_ITEM_COUNT) {
        stack_item = &stack->m_inline_items[++stack->m_item_pos];
    }
    else {
        int32_t writePos = stack->m_item_pos + 1 - LOGIC_STACK_INLINE_ITEM_COUNT;
        if (writePos >= stack->m_extern_items_capacity) {
            int32_t new_capacity;
            struct logic_stack_item * new_buf;

            new_capacity = stack->m_extern_items_capacity + 16;
            new_buf = (struct logic_stack_item *)mem_alloc(context->m_mgr->m_alloc, sizeof(struct logic_stack_item) * new_capacity);
            if (new_buf == NULL) {
                context->m_errno = -1;
                context->m_state = logic_context_state_error;
                return;
            }

            if (stack->m_extern_items) {
                memcpy(new_buf, stack->m_extern_items, sizeof(struct logic_stack_item) * stack->m_extern_items_capacity);
                mem_free(context->m_mgr->m_alloc, stack->m_extern_items);
            }

            stack->m_extern_items = new_buf;
            stack->m_extern_items_capacity = new_capacity;
        }

        assert(writePos < stack->m_extern_items_capacity);
        stack_item = &stack->m_extern_items[writePos]; 
        ++stack->m_item_pos;
    }

    assert(stack_item);
    stack_item->m_executr = executor;

    if (executor->m_type->m_category == logic_executor_category_group) {
        struct logic_executor_group * group = (struct logic_executor_group *)executor;
        if (!TAILQ_EMPTY(&group->m_members)) {
            executor = TAILQ_FIRST(&group->m_members);
            goto REINTER;
        }
    }
}

#define logic_stack_item_at(stack, pos)                                 \
    ((pos) < LOGIC_STACK_INLINE_ITEM_COUNT                              \
     ? &stack->m_inline_items[(pos)]                                    \
     : &stack->m_extern_items[(pos) - LOGIC_STACK_INLINE_ITEM_COUNT])   \

void logic_stack_exec(struct logic_stack * stack, int32_t stop_stack_pos, logic_context_t ctx) {
    while(ctx->m_state == logic_context_state_idle
          && stack->m_item_pos > stop_stack_pos
          && ctx->m_require_waiting_count == 0)
    {
        int32_t rv;
        struct logic_stack_item * stack_item = logic_stack_item_at(stack, stack->m_item_pos);

        rv = 0;

        if (stack_item->m_executr) {
            if (stack_item->m_executr->m_type->m_category == logic_executor_category_basic) {
                struct logic_executor_basic * basic = (struct logic_executor_basic *)stack_item->m_executr;
                if (basic->m_type->m_op) {
                    rv = ((logic_op_fun_t)basic->m_type->m_op)(ctx, stack_item->m_executr, basic->m_type->m_ctx, basic->m_args);
                }
                else {
                    CPE_ERROR(gd_app_em(ctx->m_mgr->m_app), "logic_stack_exec: basic logic op %s have no op!", basic->m_type->m_name);
                }
            }
            else if (stack_item->m_executr->m_type->m_category == logic_executor_category_decorate) {
                struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)stack_item->m_executr;
                if (decorator->m_type->m_op) {
                    if (((logic_decorate_fun_t)decorator->m_type->m_op)(ctx, logic_context_decorate_begin, decorator->m_type->m_ctx) == 0) {
                        logic_stack_push(stack, ctx, decorator->m_inner);
                        continue;
                    }
                }
                else {
                    CPE_ERROR(gd_app_em(ctx->m_mgr->m_app), "logic_stack_exec: decorator logic op %s have no op!", decorator->m_type->m_name);
                }
            }
        }
        else {
            CPE_ERROR(gd_app_em(ctx->m_mgr->m_app), "stack item have no executor!");
            rv = -1;
        }

        if (rv != 0) {
            logic_context_errno_set(ctx, rv);
        }

        --stack->m_item_pos;

        while(stack->m_item_pos > stop_stack_pos) {
            struct logic_stack_item * stack_item = logic_stack_item_at(stack, stack->m_item_pos);
            if (stack_item->m_executr->m_type->m_category == logic_executor_category_group) {
                if (ctx->m_errno) {
                    --stack->m_item_pos;
                    continue;
                }
                else {
                    struct logic_stack_item * last_stack_item = logic_stack_item_at(stack, stack->m_item_pos + 1);
                    logic_executor_t next = TAILQ_NEXT(last_stack_item->m_executr, m_next);
                    if (next) {
                        last_stack_item->m_executr = next;
                        ++stack->m_item_pos;
                    }
                }
            }
            else if (stack_item->m_executr->m_type->m_category == logic_executor_category_decorate) {
                struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)stack_item->m_executr;
                if (decorator->m_type->m_op) {
                    ((logic_decorate_fun_t)decorator->m_type->m_op)(ctx, logic_context_decorate_end, decorator->m_type->m_ctx);
                }
                --stack->m_item_pos;
                continue;
            }

            break;
        }
    }
}
