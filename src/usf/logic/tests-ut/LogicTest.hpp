#ifndef USF_LOGIC_TEST_LOGICTEST_H
#define USF_LOGIC_TEST_LOGICTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "gd/app/tests-env/with_app.hpp"
#include "usf/logic/tests-env/with_logic.hpp"

typedef LOKI_TYPELIST_2(
    gd::app::testenv::with_app
    , usf::logic::testenv::with_logic
    ) LogicTestBase;

class LogicTest : public testenv::fixture<LogicTestBase> {
public:
    
};

#endif
