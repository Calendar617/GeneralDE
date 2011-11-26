#include "TlTest.hpp"

TlTest::TlTest()
    : m_manage(NULL)
    , m_tl(NULL)
{
}

void TlTest::SetUp() {
    m_manage = gd_tl_manage_create(NULL);
    ASSERT_TRUE(m_manage) << "manage create fail!";
}

void TlTest::TearDown() {
    // gd_tl_manage_free(m_manage);
    // m_manage = NULL;
    // m_tl = NULL;
}

void TlTest::installTl(void) {
}
