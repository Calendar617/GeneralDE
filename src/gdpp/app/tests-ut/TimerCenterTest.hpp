#ifndef GDPP_APP_TEST_TIMERCENTERTEST_H
#define GDPP_APP_TEST_TIMERCENTERTEST_H
#include "gmock/gmock.h"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/app/tests-env/with_app.hpp"
#include "gdpp/app/TimerProcessor.hpp"
#include "gdpp/app/TimerCenter.hpp"

typedef LOKI_TYPELIST_1(Gd::App::testenv::with_app) TimerCenterTestBase;

class TimerCenterTest : public testenv::fixture<TimerCenterTestBase> {
public:
    class TimerProcessorMock : public Gd::App::TimerProcessor {
        MOCK_METHOD1(on_timer1, void(Gd::App::TimerID));
        MOCK_METHOD1(on_timer2, void(Gd::App::TimerID));
    };

    virtual void SetUp();
    virtual void TearDown();

    Gd::App::TimerCenter &  timerCenter(void);
};

#endif
