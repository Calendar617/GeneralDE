#include "gd/dp/tests-env/with_dp.hpp"

namespace gd { namespace dp { namespace testenv {

with_dp::with_dp()
    : m_dp(NULL)
{
}

void with_dp::SetUp() {
    m_dp = gd_dp_mgr_create(t_allocrator());
}


void with_dp::TearDown() {
    gd_dp_mgr_free(m_dp);
    m_dp = NULL;
}

gd_dp_mgr_t with_dp::t_dp() {
    return m_dp;
}

}}}

