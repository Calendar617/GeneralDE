#include "EventCenterTest.hpp"

TEST_F(EventCenterTest, basic) {
    EventResponserMock eventResponser;

    eventCenter().registerResponser(
        "oid-1",
        eventResponser, 
        &EventResponserMock::on_event1);

    Gd::App::Event & event = eventCenter().createEvent("event1");
    event["a"] = 1;
    eventCenter().sendEvent("oid-1", event);
    
    EXPECT_CALL(eventResponser, on_event1(::testing::StrEq("oid-1"), ::testing::Ref(event)))
        .WillOnce(::testing::Return());

    t_app_tick();

    ::testing::Mock::VerifyAndClear(&eventResponser);
}

