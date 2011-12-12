#ifndef GD_DP_TESTENV_WITHDP_H
#define GD_DP_TESTENV_WITHDP_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../dp.h"

namespace gd { namespace dp { namespace testenv {

class with_dp : public ::testenv::env<> {
public:
    with_dp();

    void SetUp();
    void TearDown();

    gd_dp_mgr_t t_dp();
private:
    gd_dp_mgr_t m_dp;
};

}}}

#endif

