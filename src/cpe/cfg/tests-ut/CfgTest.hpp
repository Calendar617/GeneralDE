#ifndef CPE_CFG_TEST_TLTEST_H
#define CPE_CFG_TEST_TLTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/cfg/cfg.h"

class CfgTest : public testenv::fixture<> {
public:
    CfgTest();

    virtual void SetUp();
    virtual void TearDown();

    cfg_t m_root;
};

#endif
