#include "CfgTest.hpp"

CfgTest::CfgTest()
    : m_root(NULL)
{
}

void CfgTest::SetUp() {
    Base::SetUp();

    m_root = cfg_create(t_allocrator());
    ASSERT_TRUE(m_root) << "manage create fail!";
}

void CfgTest::TearDown() {
    cfg_free(m_root);
    m_root = NULL;

    Base::TearDown();
}


