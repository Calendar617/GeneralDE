#ifndef CPE_OM_TEST_CLASSTEST_H
#define CPE_OM_TEST_CLASSTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "../om_class.h"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) ClassTestBase;

class ClassTest : public testenv::fixture<ClassTestBase> {
};

#endif
