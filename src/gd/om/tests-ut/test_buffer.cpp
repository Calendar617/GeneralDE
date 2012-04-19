#include "BufMgrTest.hpp"

extern struct gd_om_backend g_gd_om_backend_mem;

TEST_F(BufMgrTest, init_page_size_too_small) {
    EXPECT_EQ(-1, init(1, 40));
}

TEST_F(BufMgrTest, init_buf_size_too_small) {
    EXPECT_EQ(-1, init(41, 40));
}

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

TEST_F(BufMgrTest, get_page_buf_left) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(49);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(p == page_get());
    EXPECT_TRUE(p + 10 == page_get());
    EXPECT_TRUE(p + 20 == page_get());
    EXPECT_TRUE(p + 30 == page_get());
    EXPECT_TRUE(NULL == page_get());
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
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(p == page_get());
    EXPECT_TRUE(p + 10 == page_get());
    EXPECT_TRUE(p + 20 == page_get());
    EXPECT_TRUE(p + 30 == page_get());
    EXPECT_TRUE(NULL == page_get());
}

TEST_F(BufMgrTest, find_page_empty) {
    init(10, 40);

    EXPECT_TRUE(
        NULL ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, (void*)23121));
}

TEST_F(BufMgrTest, find_page_buf_begin) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        p ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p));
}

TEST_F(BufMgrTest, find_page_buf_middle) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        (p + 20) ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 20));
}

TEST_F(BufMgrTest, find_page_buf_last_page) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        (p + 30) ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 30));
}

TEST_F(BufMgrTest, find_page_buf_after_buf) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        NULL ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 40));
}

TEST_F(BufMgrTest, find_page_buf_before_buf) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        NULL ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p - 1));
}

TEST_F(BufMgrTest, find_page_page_middle) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        p ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 1));
}

TEST_F(BufMgrTest, find_page_page_last) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(40);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_TRUE(
        p ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 10 - 1));
}

TEST_F(BufMgrTest, find_page_multi_buf) {
    init(10, 40);

    char * p = (char *)t_tmp_alloc(80);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)(p + 40), t_em()));

    EXPECT_TRUE(
        (p + 40) ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 40));

    EXPECT_TRUE(
        (p + 70) ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 79));
}

TEST_F(BufMgrTest, find_page_multi_buf_buf_page_left) {
    init(10, 43);

    char * p = (char *)t_tmp_alloc(86);

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)p, t_em()));

    EXPECT_EQ(
        0
        , gd_om_buffer_mgr_add_new_buffer(&m_bufMgr, (gd_om_buffer_id_t)(p + 43), t_em()));

    EXPECT_TRUE(
        NULL ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 40));

    EXPECT_TRUE(
        NULL ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 42));

    EXPECT_TRUE(
        (p + 43) ==
        gd_om_buffer_mgr_find_page(&m_bufMgr, p + 43));
}
