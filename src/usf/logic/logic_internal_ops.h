#ifndef USF_LOGIC_INTERNAL_OPS_H
#define USF_LOGIC_INTERNAL_OPS_H
#include "logic_internal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*context ops*/
uint32_t logic_context_hash(const struct logic_context * context);
int logic_context_cmp(const struct logic_context * l, const struct logic_context * r);
void logic_context_free_all(logic_manage_t mgr);
void logic_context_do_state_change(logic_context_t context, logic_context_state_t old_sate);

#define logic_context_state_i(context)                  \
    (context->m_errno                                   \
     ? logic_context_state_error                        \
     : ((context->m_state == logic_context_state_idle   \
         && context->m_require_waiting_count)           \
         ? logic_context_state_waiting                  \
         : context->m_state))

/*require ops*/
uint32_t logic_require_hash(const struct logic_require * require);
int logic_require_cmp(const struct logic_require * l, const struct logic_require * r);
void logic_require_free_all(logic_manage_t mgr);

/*data ops*/
uint32_t logic_data_hash(const struct logic_data * data);
int logic_data_cmp(const struct logic_data * l, const struct logic_data * r);
void logic_data_free_all(logic_manage_t mgr);

/*require type ops*/
uint32_t logic_require_type_hash(const struct logic_require_type * require);
int logic_require_type_cmp(const struct logic_require_type * l, const struct logic_require_type * r);
void logic_require_type_free_all(logic_manage_t mgr);

/*stack ops*/
void logic_stack_init(struct logic_stack * stack);
void logic_stack_fini(struct logic_stack * stack, logic_context_t context);
void logic_stack_push(struct logic_stack * stack, logic_context_t context, logic_executor_t executor);
    
void logic_stack_exec(struct logic_stack * stack, int32_t stop_stack_pos, logic_context_t ctx);

#ifdef __cplusplus
}
#endif

#endif
