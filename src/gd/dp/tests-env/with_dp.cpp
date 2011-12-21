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

gd_dp_req_t
with_dp::t_dp_req_create(const char * type, size_t capacity) {
    return gd_dp_req_create(t_dp(), cpe_hs_create(t_tmp_allocrator(), type), capacity);
}

gd_dp_req_t
with_dp::t_dp_req_create_child(gd_dp_req_t req, const char * type, void * buf, size_t capacity) {
    return gd_dp_req_create_child(req, cpe_hs_create(t_tmp_allocrator(), type), buf, capacity);
}

}}}

