#ifndef GDPP_APP_TEST_EVENTCENTERTEST_H
#define GDPP_APP_TEST_EVENTCENTERTEST_H
#include "gmock/gmock.h"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/app/tests-env/with_app.hpp"
#include "gdpp/app/EventResponser.hpp"
#include "../EventCenterExt.hpp"

typedef LOKI_TYPELIST_1(Gd::App::testenv::with_app) EventCenterTestBase;

class EventCenterTest : public testenv::fixture<EventCenterTestBase> {
public:
    class EventResponserMock : public Gd::App::EventResponser {
    public:
        MOCK_METHOD2(on_event1, void(const char * oid, Gd::App::Event const & e));
        MOCK_METHOD2(on_event2, void(const char * oid, Gd::App::Event const & e));
    };

    virtual void SetUp();
    virtual void TearDown();

    Gd::App::EventCenterExt &  eventCenter(void);
    void tick(void);
};

#endif
