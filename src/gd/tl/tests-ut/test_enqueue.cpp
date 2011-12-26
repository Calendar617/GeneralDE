#include "TlTest.hpp"

class EnqueueTest : public TlTest {
public:
    void SetUp() {
        TlTest::SetUp();
        installTl();
    }
};

TEST_F(EnqueueTest, basic) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    EXPECT_TRUE(!gd_tl_event_in_queue(event));

    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(event, 0, 0, 1));

    EXPECT_TRUE(gd_tl_event_in_queue(event));
}

TEST_F(EnqueueTest, delay_negative) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(event, -1, 0, 1));
}

TEST_F(EnqueueTest, span_zero_not_repeat) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(event, 0, 0, 1));

    EXPECT_TRUE(gd_tl_event_in_queue(event));
}

TEST_F(EnqueueTest, span_zero_repeat) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(createEvent(0), 0, 0, 20));
}

TEST_F(EnqueueTest, span_zero_repeat_negative) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(createEvent(0), 0, 0, -1));
}

TEST_F(EnqueueTest, span_negative) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(createEvent(0), 0, -1, 20));
}

TEST_F(EnqueueTest, repeatCount_negative) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(event, 0, 1, -1));

    EXPECT_TRUE(gd_tl_event_in_queue(event));
}

TEST_F(EnqueueTest, repeatCount_zero) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(createEvent(0), 0, 1, 0));
}

TEST_F(EnqueueTest, event_null) {
    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(NULL, 0, 0, 1));
}

TEST_F(EnqueueTest, event_tl_null) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    event->m_tl = NULL;

    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(event, 0, 0, 1));

    event->m_tl = m_tl;
}

TEST_F(EnqueueTest, event_tm_null) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    event->m_tl->m_manage = NULL;

    EXPECT_EQ(
        GD_TL_ERROR_BAD_ARG,
        gd_tl_event_send_ex(event, 0, 0, 1));

    event->m_tl->m_manage = m_manage;
}

TEST_F(EnqueueTest, event_not_in_building_queue) {
    gd_tl_event_t event = createEvent(0);
    ASSERT_TRUE(event);

    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(event, 0, 0, 1));

    EXPECT_EQ(
        GD_TL_ERROR_EVENT_UNKNOWN,
        gd_tl_event_send_ex(event, 0, 0, 1));
}
