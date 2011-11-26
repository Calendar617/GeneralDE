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
    gd_tl_manage_free(m_manage);
    m_manage = NULL;
    m_tl = NULL;
}

void TlTest::installTl(void) {
    EXPECT_TRUE(m_manage);
    m_tl = gd_tl_create(m_manage);
}

gd_tl_event_t
TlTest::createEvent(size_t capacity) {
    EXPECT_TRUE(m_tl != NULL);
    if (m_tl == NULL) return NULL;

    return gd_tl_event_create(m_tl, capacity);
}
