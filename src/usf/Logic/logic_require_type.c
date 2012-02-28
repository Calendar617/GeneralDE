#include <assert.h>
#include "usf/logic/logic_require_type.h"
#include "logic_internal_ops.h"

uint32_t logic_require_type_hash(const struct logic_require_type * rt) {
    return cpe_hs_value(rt->m_name);
}

int logic_require_type_cmp(const struct logic_require_type * l, const struct logic_require_type * r) {
    return cpe_hs_cmp(l->m_name, r->m_name) == 0 ? 1 : 0;
}
