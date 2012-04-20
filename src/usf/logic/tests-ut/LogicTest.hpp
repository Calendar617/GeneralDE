#ifndef USF_LOGIC_TEST_LOGICTEST_H
#define USF_LOGIC_TEST_LOGICTEST_H
#include <string>
#include "gmock/gmock.h"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/cfg/tests-env/with_cfg.hpp"
#include "gd/app/tests-env/with_app.hpp"
#include "usf/logic/tests-env/with_logic.hpp"

typedef LOKI_TYPELIST_4(
    utils::testenv::with_em
    , gd::app::testenv::with_app
    , cpe::cfg::testenv::with_cfg
    , usf::logic::testenv::with_logic
    ) LogicTestBase;

class LogicTest : public testenv::fixture<LogicTestBase> {
public:
    class LogicOpMock {
    public:
        MOCK_METHOD1(execute, int32_t(logic_context_t ctx));
    };

    class CommitMock {
    public:
        MOCK_METHOD1(commit, void(logic_context_t ctx));
    };

    class RequireTypeMock {
    public:
        MOCK_METHOD1(cancel, void(logic_require_t require));
        MOCK_METHOD1(destory, void(logic_require_t require));
    };

    void SetUp();
    void TearDown();

    using Base::t_logic_executor_build;

    LogicOpMock & installOp(const char * name);
    LogicOpMock & op(const char * name);

    void set_commit(logic_context_t context, CommitMock & mock);
    void set_destory(logic_require_type_t rt, RequireTypeMock & mock);
    void set_cancel(logic_require_type_t rt, RequireTypeMock & mock);
};

#endif
