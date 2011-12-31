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

int BufMgrTest::init(size_t page_size, size_t buf_size) {
    if (m_inited) {
        gd_om_buffer_mgr_fini(&m_bufMgr);
        m_inited = false;
    }

    int r = gd_om_buffer_mgr_init(
        &m_bufMgr,
        page_size,
        buf_size,
        t_allocrator());

    if (r == 0) {
        m_inited = true;
    }

    return r;
}

void * BufMgrTest::page_get(void) {
    EXPECT_TRUE(m_inited);

    return gd_om_page_get(&m_bufMgr, t_em());
}


