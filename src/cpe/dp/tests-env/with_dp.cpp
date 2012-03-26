#include "cpe/dp/tests-env/with_dp.hpp"

namespace gd { namespace dp { namespace testenv {

with_dp::with_dp()
    : m_dp(NULL)
{
}

void with_dp::SetUp() {
    m_dp = dp_mgr_create(t_allocrator());
}


void with_dp::TearDown() {
    dp_mgr_free(m_dp);
    m_dp = NULL;
}

dp_mgr_t with_dp::t_dp() {
    return m_dp;
}

dp_req_t
with_dp::t_dp_req_create(const char * type, size_t capacity) {
    return dp_req_create(t_dp(), cpe_hs_create(t_tmp_allocrator(), type), capacity);
}

dp_req_t
with_dp::t_dp_req_create_child(dp_req_t req, const char * type, void * buf, size_t capacity) {
    dp_req_t r = dp_req_create_with_buf(m_dp, cpe_hs_create(t_tmp_allocrator(), type), buf, capacity);
    dp_req_set_parent(r, req);
    return r;
}

}}}

