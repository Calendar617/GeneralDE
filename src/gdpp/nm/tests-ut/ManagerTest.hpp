#ifndef GDPP_NM_TEST_MANAGERTEST_H
#define GDPP_NM_TEST_MANAGERTEST_H
#include "gd/nm/tests-env/with_nm.hpp"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/nm/Object.hpp"

typedef LOKI_TYPELIST_1(gd::nm::testenv::with_nm) ManagerTestBase;

class ManagerTest : public testenv::fixture<ManagerTestBase> {
public:
    ManagerTest();

    int _destoryCount;
};

#endif
