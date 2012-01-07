#include <assert.h>
#include "gd/tl/tl_manage.h"
#include "gd/evt/evt_manage.h"
#include "evt_internal_types.h"

gd_evt_mgr_t
gd_evt_mgr_create(
    gd_tl_manage_t tlm,
    LPDRMETALIB metalib,
    mem_allocrator_t alloc)
{
    gd_evt_mgr_t evm;

    evm = (gd_evt_mgr_t)mem_alloc(alloc, sizeof(struct gd_evt_mgr));
    if (evm == NULL) return NULL;

    evm->m_alloc = alloc;
    evm->m_metalib = metalib;
    evm->m_tl = gd_tl_create(tlm);
    if (evm->m_tl == NULL) {
        mem_free(alloc, evm);
        return NULL;
    }

    return evm;
}

void gd_evt_mgr_free(gd_evt_mgr_t evm) {
    if (evm == NULL) return;

    mem_free(evm->m_alloc, evm);
}

LPDRMETALIB
gd_evt_mgr_metalib(gd_evt_mgr_t evm) {
    assert(evm);
    return evm->m_metalib;
}
