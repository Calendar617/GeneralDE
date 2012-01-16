#include "TlTest.hpp"

TEST_F(TlTest, time_init_default) {
    EXPECT_NEAR(
        gd_tl_time_source_msec(NULL),
        gd_tl_time_source_msec(NULL) + 10,
        gd_tl_manage_time(m_manage)
        );
}

TEST_F(TlTest, manage_setopt_time_source_basic) {
    gd_tl_time_fun_t newSouce = gd_tl_time_source_usec;
    EXPECT_TRUE(m_manage->m_time_get != newSouce);

    EXPECT_EQ(
        0,
        gd_tl_manage_set_opt(
            m_manage, gd_tl_set_time_source
            , newSouce));

    EXPECT_TRUE(
        m_manage->m_time_get == newSouce);

    EXPECT_NEAR(
        newSouce(NULL),
        newSouce(NULL) + 10,
        gd_tl_manage_time(m_manage)
        );
}

TEST_F(TlTest, manage_setopt_time_source_NULL) {
    EXPECT_EQ(
        GD_TL_ERROR_NO_TIME_SOURCE,
        gd_tl_manage_set_opt(
            m_manage, gd_tl_set_time_source
            , NULL));

    EXPECT_TRUE(
        m_manage->m_time_get == gd_tl_time_source_msec);
}

TEST_F(TlTest, manage_setopt_time_context) {
    EXPECT_EQ(
        0,
        gd_tl_manage_set_opt(
            m_manage, gd_tl_set_time_source_context
            , (void*)15));

    EXPECT_TRUE((void*)15 == m_manage->m_time_ctx);
}

TEST_F(TlTest, manage_setopt_time_cvt) {
    EXPECT_EQ(
        0,
        gd_tl_manage_set_opt(
            m_manage, gd_tl_set_time_cvt
            , gd_tl_time_cvt_sec2usec));

    EXPECT_TRUE(gd_tl_time_cvt_sec2usec == m_manage->m_time_cvt);
}

TEST_F(TlTest, manage_setopt_unknown) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_manage_set_opt(
            m_manage, (gd_tl_manage_option_t)123));
}

TEST_F(TlTest, manage_create_tl) {
    for(int i = 0; i < 100; ++i) {
        EXPECT_TRUE(gd_tl_create(m_manage) != NULL);
    }
}
