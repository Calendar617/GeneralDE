#include "BufMgrTest.hpp"

BufMgrTest::BufMgrTest() : m_inited(false) {
}

void BufMgrTest::SetUp() {
    Base::SetUp();
}

void BufMgrTest::TearDown() {
    if (m_inited) {
        gd_om_buffer_mgr_fini(&m_bufMgr);
        m_inited = false;
    }

    Base::TearDown();
}

void BufMgrTest::init(size_t page_size, size_t buf_size) {
    if (m_inited) {
        gd_om_buffer_mgr_fini(&m_bufMgr);
        m_inited = false;
    }

    ASSERT_EQ(
        0, 
        gd_om_buffer_mgr_init(
            &m_bufMgr,
            page_size,
            buf_size,
            t_allocrator()));

    m_inited = true;
}

void * BufMgrTest::page_get(void) {
    EXPECT_TRUE(m_inited);

    return gd_om_page_get(&m_bufMgr, t_em());
}


