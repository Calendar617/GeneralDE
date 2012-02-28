#include "usf/logic/logic_context.h"
#include "logic_internal_ops.h"

logic_context_state_t
logic_context_state(logic_context_t context);

uint32_t logic_context_hash(const struct logic_context * context) {
    return context->m_id;
}

int logic_context_cmp(const struct logic_context * l, const struct logic_context * r) {
    return l->m_id == r->m_id;
}
