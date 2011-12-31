#include "BufMgrTest.hpp"

extern struct gd_om_backend g_gd_om_backend_mem;

TEST_F(BufMgrTest, get_page_empty_no_source) {
    init(10, 40);
    EXPECT_TRUE(NULL == page_get());

    EXPECT_TRUE(t_em_have_errno(gd_om_no_buffer));
}

TEST_F(BufMgrTest, get_page_auto_create) {
    init(10, 40);
    EXPECT_EQ(0, gd_om_buffer_mgr_set_backend(&m_bufMgr, &g_gd_om_backend_mem, t_allocrator()));

    EXPECT_TRUE(page_get());
}

TEST_F(BufMgrTest, set_source_no_buf) {
    init(10, 40);
    EXPECT_EQ(0, gd_om_buffer_mgr_set_backend(&m_bufMgr, &g_gd_om_backend_mem, t_allocrator()));
    EXPECT_EQ(0, gd_om_buffer_mgr_set_backend(&m_bufMgr, NULL, NULL));
}

TEST_F(BufMgrTest, set_source_have_buf) {
    init(10, 40);
    EXPECT_EQ(0, gd_om_buffer_mgr_set_backend(&m_bufMgr, &g_gd_om_backend_mem, t_allocrator()));

    EXPECT_TRUE(page_get());

    EXPECT_EQ(-1, gd_om_buffer_mgr_set_backend(&m_bufMgr, NULL, NULL));
}

TEST_F(BufMgrTest, add_page_no_source) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(p == page_get());
    EXPECT_TRUE(p + 10 == page_get());
    EXPECT_TRUE(p + 20 == page_get());
    EXPECT_TRUE(p + 30 == page_get());
    EXPECT_TRUE(NULL == page_get());
}
