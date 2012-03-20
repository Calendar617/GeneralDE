#include <vector>
#include "EventCenterTest.hpp"

class EventResponser_RegistInEvent : public Gd::App::EventResponser {
public:
    EventResponser_RegistInEvent(Gd::App::EventCenter & ec)
        : m_ec(ec)
    {
    }

    void on_event(const char * oid, Gd::App::Event const & e) {
        m_ec.registerResponser(
            "oid-1",
            *this, 
            &EventResponser_RegistInEvent::on_event);
    }

    Gd::App::EventCenter & m_ec;
};

TEST_F(EventCenterTest, register_in_process) {
    EventResponser_RegistInEvent eventResponser(eventCenter());

    eventCenter().registerResponser(
        "oid-1",
        eventResponser, 
        &EventResponser_RegistInEvent::on_event);

    Gd::App::Event & event = eventCenter().createEvent("event1");
    event["a"] = 1;
    eventCenter().sendEvent("oid-1", event);
    
    t_app_tick();
}

class EventResponser_UnregistInEvent : public Gd::App::EventResponser {
public:
    EventResponser_UnregistInEvent(Gd::App::EventCenter & ec, Gd::App::EventResponser & r)
        : m_ec(ec), m_r(r)
    {
    }

    void on_event(const char * oid, Gd::App::Event const & e) {
        m_ec.unregisterResponser(m_r);
    }

    Gd::App::EventCenter & m_ec;
    Gd::App::EventResponser & m_r;
};

TEST_F(EventCenterTest, unregister_in_process) {
    EventResponserMock otherResponser;
    eventCenter().registerResponser(
        "oid-1",
        otherResponser, 
        &EventResponserMock::on_event1);
    EXPECT_CALL(otherResponser, on_event1(::testing::StrEq("oid-1"), ::testing::_ ))
        .WillOnce(::testing::Return());

    EventResponser_UnregistInEvent eventResponser(eventCenter(), otherResponser);

    eventCenter().registerResponser(
        "oid-1",
        eventResponser, 
        &EventResponser_UnregistInEvent::on_event);

    Gd::App::Event & event = eventCenter().createEvent("event1");
    event["a"] = 1;
    eventCenter().sendEvent("oid-1", event);
    
    t_app_tick();
}

class EventResponser_UnregistSelfInEvent : public Gd::App::EventResponser {
public:
    EventResponser_UnregistSelfInEvent(Gd::App::EventCenter & ec) : m_ec(ec) {
    }

    void on_event(const char * oid, Gd::App::Event const & e) {
        m_ec.unregisterResponser(*this);
    }

    Gd::App::EventCenter & m_ec;
};

TEST_F(EventCenterTest, unregister_self_in_process) {
    EventResponserMock er1;
    eventCenter().registerResponser(
        "oid-1",
        er1, 
        &EventResponserMock::on_event1);
    EXPECT_CALL(er1, on_event1(::testing::StrEq("oid-1"), ::testing::_))
        .WillOnce(::testing::Return());

    EventResponser_UnregistSelfInEvent eventResponser(eventCenter());

    eventCenter().registerResponser(
        "oid-1",
        eventResponser, 
        &EventResponser_UnregistSelfInEvent::on_event);
    
    EventResponserMock er2;
    eventCenter().registerResponser(
        "oid-1",
        er2, 
        &EventResponserMock::on_event1);
    EXPECT_CALL(er2, on_event1(::testing::StrEq("oid-1"), ::testing::_))
        .WillOnce(::testing::Return());

    Gd::App::Event & event = eventCenter().createEvent("event1");
    event["a"] = 1;
    eventCenter().sendEvent("oid-1", event);

    t_app_tick();
}

