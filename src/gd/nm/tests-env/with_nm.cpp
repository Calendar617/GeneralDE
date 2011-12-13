#include "gd/nm/tests-env/with_nm.hpp"

namespace gd { namespace nm { namespace testenv {

with_nm::with_nm()
    : m_nm(NULL)
{
}

void with_nm::SetUp() {
    m_nm = gd_nm_mgr_create(t_allocrator(), 8);
}


void with_nm::TearDown() {
    gd_nm_mgr_free(m_nm);
    m_nm = NULL;
}

gd_nm_mgr_t with_nm::t_nm() {
    return m_nm;
}

}}}

