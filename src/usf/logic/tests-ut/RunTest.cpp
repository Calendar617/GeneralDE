#include "usf/logic/logic_require_type.h"
#include "usf/logic/logic_require.h"
#include "RunTest.hpp"

RunTest::RunTest()
    : m_context(0)
    , m_executor(0)
{
}

void RunTest::SetUp() {
    LogicTest::SetUp();
    logic_require_type_create(t_logic_manager(), "r1");
    m_context = t_logic_context_create();
    set_commit(m_context, m_commitMock);
}

void RunTest::TearDown() {
    if (m_executor) {
        logic_executor_free(m_executor);
        m_executor = NULL;
    }

    LogicTest::TearDown();
}

static void create_rquire(logic_context_t context) {
    char name[] = CPE_HS_BUF_MAKE("r1");
    logic_require_create(context, (cpe_hash_string_t)name, 0);
}

void RunTest::expect_create_require(LogicOpMock & op) {
    EXPECT_CALL(op, execute(::testing::_))
        .WillOnce(
            ::testing::DoAll(
                ::testing::Invoke(create_rquire),
                ::testing::Return(0)));
}

void RunTest::expect_return(LogicOpMock & op, int32_t rv) {
    EXPECT_CALL(op, execute(::testing::_))
        .WillOnce(::testing::Return(rv));
}

void RunTest::execute(const char * data) {
    m_executor = t_logic_executor_build(data);
    t_logic_execute(m_context, m_executor);
}

void RunTest::execute_again(void) {
    logic_context_execute(m_context);
}

void RunTest::expect_commit(void) {
    EXPECT_CALL(m_commitMock, commit(::testing::_));
}

void RunTest::set_require_keep(void) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);
}

void RunTest::set_execute_immediately(void) {
    logic_context_flag_enable(m_context, logic_context_flag_execute_immediately);
}

logic_context_state_t
RunTest::state(void) {
    return logic_context_state(m_context);
}

int32_t RunTest::rv(void) {
    return logic_context_errno(m_context);
}

void RunTest::cancel(void) {
    logic_context_cancel(m_context);
}

void RunTest::timeout(void) {
    logic_context_timeout(m_context);
}
