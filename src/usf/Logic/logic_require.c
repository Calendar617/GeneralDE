#include <assert.h>
#include "usf/logic/logic_require.h"
#include "logic_internal_ops.h"

uint32_t logic_require_hash(const struct logic_require * require) {
    return require->m_id;
}

int logic_require_cmp(const struct logic_require * l, const struct logic_require * r) {
    return l->m_id == r->m_id;
}
