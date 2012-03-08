#ifndef CPE_DR_TOOLS_TEST_GENERATETEST_H
#define CPE_DR_TOOLS_TEST_GENERATETEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) GenerateTestBase;

class GenerateTest : public testenv::fixture<GenerateTestBase> {
public:
    GenerateTest();
    virtual void SetUp();
    virtual void TearDown();
};

#endif
