#include <sstream>
#include "TlTest.hpp"

class DispatchTest : public TlTest {
public:
    void SetUp() {
        TlTest::SetUp();
        installTl();

        gd_tl_manage_set_opt(m_manage, gd_tl_set_time_source_context, m_manage);
        gd_tl_manage_set_opt(m_manage, gd_tl_set_time_source, gd_tl_time_source_last_event);

        gd_tl_set_opt(m_tl, gd_tl_set_event_op_context, this);
        gd_tl_set_opt(m_tl, gd_tl_set_event_dispatcher, event_dispatch);
    }

    static void event_dispatch(gd_tl_event_t event, void * context) {
        ((DispatchTest*)context)->executeS
            << ":" << (char*)gd_tl_event_data(event);
    }

    void addEvent(const char * value, gd_tl_time_span_t delay,  gd_tl_time_span_t span, int repeatCount) {
        gd_tl_event_t event = gd_tl_event_create(m_tl, strlen(value) + 1);
        memcpy(gd_tl_event_data(event), value, gd_tl_event_capacity(event));
        EXPECT_EQ(0, gd_tl_event_send_ex(event, delay, span, repeatCount));
    }

    void addAction(const char * value) {
        gd_tl_event_t event = gd_tl_action_add(m_tl);
        ASSERT_LT(strlen(value) + 1, gd_tl_event_capacity(event));
        memcpy(gd_tl_event_data(event), value, strlen(value) + 1);
    }

    ::std::stringstream executeS;
};

TEST_F(DispatchTest, empty) {
    EXPECT_EQ(
        0,
        gd_tl_manage_tick(m_manage, 10));
}

TEST_F(DispatchTest, action_basic) {
    addAction("a");
    EXPECT_EQ(1, gd_tl_manage_tick(m_manage, 10));
    EXPECT_STREQ(":a", executeS.str().c_str());
}

TEST_F(DispatchTest, action_order) {
    addAction("a");
    addAction("b");
    addAction("c");

    EXPECT_EQ(3, gd_tl_manage_tick(m_manage, 10));
    EXPECT_STREQ(":a:b:c", executeS.str().c_str());
}

TEST_F(DispatchTest, action_execute_count) {
    addAction("a");
    addAction("b");
    addAction("c");

    EXPECT_EQ(2, gd_tl_manage_tick(m_manage, 2));
    EXPECT_STREQ(":a:b", executeS.str().c_str());
}

TEST_F(DispatchTest, event_basic) {
    addEvent("a", 0, 0, 1);
    EXPECT_EQ(1, gd_tl_manage_tick(m_manage, 10));
    EXPECT_STREQ(":a", executeS.str().c_str());
}

TEST_F(DispatchTest, event_order_same_time) {
    addEvent("a", 0, 0, 1);
    addEvent("b", 0, 0, 1);
    addEvent("c", 0, 0, 1);

    EXPECT_EQ(3, gd_tl_manage_tick(m_manage, 10));
    EXPECT_STREQ(":a:b:c", executeS.str().c_str());
}

TEST_F(DispatchTest, event_order_by_time) {
    addEvent("c", 2, 0, 1);
    addEvent("b", 1, 0, 1);
    addEvent("a", 0, 0, 1);

    EXPECT_EQ(3, gd_tl_manage_tick(m_manage, 10));
    EXPECT_STREQ(":a:b:c", executeS.str().c_str());
}

