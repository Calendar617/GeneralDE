#ifndef GD_NM_TESTENV_WITHDP_H
#define GD_NM_TESTENV_WITHDP_H
#include "cpe/utils/tests-env/test-env.hpp"
#include "../nm.h"

namespace gd { namespace nm { namespace testenv {

class with_nm : public ::testenv::env<> {
public:
    with_nm();

    void SetUp();
    void TearDown();

    gd_nm_mgr_t t_nm();
private:
    gd_nm_mgr_t m_nm;
};

}}}

#endif

