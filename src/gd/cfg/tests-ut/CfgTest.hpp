#ifndef GD_CFG_TEST_TLTEST_H
#define GD_CFG_TEST_TLTEST_H
#include "gtest/gtest.h"
#include "gd/cfg/cfg.h"

class CfgTest : public ::testing::Test {
public:
    CfgTest();

    virtual void SetUp();
    virtual void TearDown();

    gd_cfg_t m_root;
};

#endif
