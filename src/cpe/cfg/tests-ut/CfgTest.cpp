#include "CfgTest.hpp"

CfgTest::CfgTest()
    : m_root(NULL)
{
}

void CfgTest::SetUp() {
    m_root = cfg_create(NULL);
    ASSERT_TRUE(m_root) << "manage create fail!";
}

void CfgTest::TearDown() {
    cfg_free(m_root);
    m_root = NULL;
}

