#include <assert.h>
#include "cpe/dr/dr_metalib_init.h"
#include "../dr_internal_types.h"

void dr_lib_free(INOUT LPDRMETALIB* ppstLib) {
    if (*ppstLib) {
        free(*ppstLib);
        *ppstLib = NULL;
    }
}

LPDRMETALIB dr_lib_clone(OUT void* a_pvDst, INOUT int* a_piBuff, IN LPDRMETALIB a_pstSrcLib) {
    return 0;
}

size_t dr_lib_size(IN LPDRMETALIB pstLib) {
    assert(pstLib);
    return pstLib->m_size;
}

const char *dr_lib_name(IN LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_name;
}

int dr_lib_version(IN LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_version;
}

int dr_lib_build_version(IN LPDRMETALIB a_pstLib) {
    assert(a_pstLib);
    return a_pstLib->m_build_version;
}
