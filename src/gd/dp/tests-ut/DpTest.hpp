#ifndef CPE_DR_TEST_HASHTEST_H
#define CPE_DR_TEST_HASHTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gd/dp/tests-env/with_dp.hpp"

typedef LOKI_TYPELIST_2(
    gd::dp::testenv::with_dp,
    utils::testenv::with_em) DpTestBase;

class DpTest : public testenv::fixture<DpTestBase> {
public:
};

#endif
