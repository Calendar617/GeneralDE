#include <stdlib.h>
#include <assert.h>
#include "cpe/dr/dr_metalib_init.h"
#include "../dr_internal_types.h"

size_t dr_lib_size(LPDRMETALIB pstLib) {
    assert(pstLib);
    return pstLib->m_size;
}

const char *dr_lib_name(LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_name;
}

int dr_lib_version(LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_version;
}

int dr_lib_build_version(LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_build_version;
}
