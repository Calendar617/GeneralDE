#include <assert.h>
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
                context->m_state = logic_context_error;
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

    if (executor->m_type == logic_executor_type_group) {
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
    while(ctx->m_state == logic_context_idle
          && stack->m_item_pos > stop_stack_pos)
    {
        int32_t rv;
        struct logic_stack_item * stack_item = logic_stack_item_at(stack, stack->m_item_pos);

        rv = 0;

        if (stack_item->m_executr->m_type == logic_executor_type_basic) {
            struct logic_executor_basic * basic = (struct logic_executor_basic *)stack_item->m_executr;
            rv = basic->m_op(ctx, basic->m_ctx, basic->m_args);
        }
        else if (stack_item->m_executr->m_type == logic_executor_type_decorate) {
            //struct logic_executor_decorate * decorator = (struct logic_executor_decorate *)stack_item->m_executr;
            //rv = decorator->m_op(app, ctx, stack_item->m_executr, decorator->m_ctx);
        }

        if (rv != 0) {
            ctx->m_errno = rv;
            ctx->m_state = logic_context_error;
        }

        --stack->m_item_pos;

        if (stack->m_item_pos >= 0) {
            struct logic_stack_item * stack_item = logic_stack_item_at(stack, stack->m_item_pos);
            if (stack_item->m_executr->m_type == logic_executor_type_group) {
                struct logic_stack_item * last_stack_item = logic_stack_item_at(stack, stack->m_item_pos + 1);
                logic_executor_t next = TAILQ_NEXT(last_stack_item->m_executr, m_next);
                if (next) {
                    last_stack_item->m_executr = next;
                }
            }
            else if (stack_item->m_executr->m_type == logic_executor_type_decorate) {
                
            }
        }
    }
}
