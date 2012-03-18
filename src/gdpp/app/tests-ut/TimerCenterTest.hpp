#ifndef GDPP_APP_TEST_TIMERCENTERTEST_H
#define GDPP_APP_TEST_TIMERCENTERTEST_H
#include "gmock/gmock.h"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/app/tests-env/with_app.hpp"
#include "gdpp/app/TimerProcessor.hpp"
#include "../TimerCenterExt.hpp"

typedef LOKI_TYPELIST_2(
    utils::testenv::with_em,
    Gd::App::testenv::with_app) TimerCenterTestBase;

class TimerCenterTest : public testenv::fixture<TimerCenterTestBase> {
public:
    class TimerProcessorMock : public Gd::App::TimerProcessor {
    public:
        MOCK_METHOD1(on_timer1, void(Gd::App::TimerID));
        MOCK_METHOD1(on_timer2, void(Gd::App::TimerID));
    };

    virtual void SetUp();
    virtual void TearDown();

    Gd::App::TimerCenterExt &  timerCenter(void);
    void tick(void);
};

#endif
