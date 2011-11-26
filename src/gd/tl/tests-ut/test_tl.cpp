#include "TlTest.hpp"

class TimeLineTest : public TlTest {
    void SetUp() {
        TlTest::SetUp();
        m_tl = gd_tl_create(m_manage);
    }

    void TearDown() {
        m_tl = NULL;
        TlTest::SetUp();
    }
};

TEST_F(TimeLineTest, setopt_dispatcher) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_dispatcher,
            (gd_tl_event_dispatcher_t)12));

    EXPECT_TRUE(
        m_tl->m_event_dispatcher == (gd_tl_event_dispatcher_t)12);
}

TEST_F(TimeLineTest, setopt_dispatcher_null) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_dispatcher,
            (gd_tl_event_dispatcher_t)0));

    EXPECT_TRUE(
        m_tl->m_event_dispatcher == (gd_tl_event_dispatcher_t)0);
}

TEST_F(TimeLineTest, setopt_enqueue) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_enqueue,
            (gd_tl_event_enqueue_t)12));

    EXPECT_TRUE(
        m_tl->m_event_enqueue == (gd_tl_event_enqueue_t)12);
}

TEST_F(TimeLineTest, setopt_enqueue_null) {
    EXPECT_EQ(
        GD_TL_ERROR_EVENT_NO_ENQUEUE,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_enqueue,
            (gd_tl_event_enqueue_t)0));

    EXPECT_TRUE(
        m_tl->m_event_enqueue == gd_tl_event_enqueue_local);
}

TEST_F(TimeLineTest, setopt_destory) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_destory,
            (gd_tl_event_destory_t)12));

    EXPECT_TRUE(
        m_tl->m_event_destory == (gd_tl_event_destory_t)12);
}

TEST_F(TimeLineTest, setopt_destory_null) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_destory,
            (gd_tl_event_destory_t)0));

    EXPECT_TRUE(
        m_tl->m_event_destory == (gd_tl_event_destory_t)0);
}

TEST_F(TimeLineTest, setopt_op_context) {
    EXPECT_EQ(
        0,
        gd_tl_set_opt(
            m_tl, gd_tl_set_event_op_context,
            (void*)12));

    EXPECT_TRUE(
        m_tl->m_event_op_context == (void*)12);
}

TEST_F(TlTest, setopt_unknown) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_set_opt(
            m_tl, (gd_tl_option_t)123));
}
