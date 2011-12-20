#include "TlTest.hpp"

class EventTest : public TlTest {
public:
    void SetUp() {
        TlTest::SetUp();
        installTl();
        construct_count = destory_count = 0;

        gd_tl_manage_set_opt(m_manage, gd_tl_set_time_source_context, m_manage);
        gd_tl_manage_set_opt(m_manage, gd_tl_set_time_source, gd_tl_time_source_last_event);

        gd_tl_set_opt(m_tl, gd_tl_set_event_op_context, this);
        gd_tl_set_opt(m_tl, gd_tl_set_event_dispatcher, event_dispatch);
        gd_tl_set_opt(m_tl, gd_tl_set_event_construct, event_construct);
        gd_tl_set_opt(m_tl, gd_tl_set_event_destory, event_destory);
    }

    int construct_count;
    int destory_count;

    static void event_dispatch(gd_tl_event_t event, void * context) {}
    static void event_construct(gd_tl_event_t event, void * context) { ++((EventTest *)context)->construct_count; }
    static void event_destory(gd_tl_event_t event, void * context) { ++((EventTest *)context)->destory_count; }
        
};

TEST_F(EventTest, event_data_basic) {
    gd_tl_event_t event = createEvent(12);
    ASSERT_TRUE(event);

    EXPECT_EQ((size_t)12, gd_tl_event_capacity(event));
    EXPECT_TRUE(m_tl == gd_tl_event_tl(event));
}

TEST_F(EventTest, event_construct) {
    createEvent(0);

    EXPECT_EQ(1, construct_count);
}

TEST_F(EventTest, event_destory_not_commited_in_tick) {
    createEvent(0);

    EXPECT_EQ(0, gd_tl_manage_tick(m_manage, -1));
    EXPECT_EQ(1, destory_count);
}

TEST_F(EventTest, event_destory_commited_in_tick) {
    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(createEvent(0), 0, 0, 1));

    EXPECT_EQ(1, gd_tl_manage_tick(m_manage, -1));

    EXPECT_EQ(1, destory_count);
}

TEST_F(EventTest, event_destory_not_commited_in_free) {
    createEvent(0);

    gd_tl_manage_free(m_manage);
    m_manage = NULL;

    EXPECT_EQ(1, destory_count);
}

TEST_F(EventTest, event_destory_commited_in_free) {
    EXPECT_EQ(
        0,
        gd_tl_event_send_ex(createEvent(0), 0, 0, 1));

    gd_tl_manage_free(m_manage);
    m_manage = NULL;

    EXPECT_EQ(1, destory_count);
}

TEST_F(EventTest, action_data_basic) {
    gd_tl_event_t event = createAction();
    ASSERT_TRUE(event);

    EXPECT_EQ(
        (size_t)(GD_TL_ACTION_SIZE - sizeof(struct gd_tl_event)),
        gd_tl_event_capacity(event));
    EXPECT_TRUE(m_tl == gd_tl_event_tl(event));
}

TEST_F(EventTest, action_construct) {
    createAction();

    EXPECT_EQ(1, construct_count);
}

TEST_F(EventTest, action_destory_in_tick) {
    createAction();

    EXPECT_EQ(1, gd_tl_manage_tick(m_manage, -1));
    EXPECT_EQ(1, destory_count);
}

TEST_F(EventTest, action_destory_in_free) {
    createAction();

    gd_tl_manage_free(m_manage);
    m_manage = NULL;

    EXPECT_EQ(1, destory_count);
}
