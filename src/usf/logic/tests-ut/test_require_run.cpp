#include "usf/logic/logic_require.h"
#include "RunTest.hpp"

class RequireRunTest : public RunTest {
public:
    void set_require_done(void) {
        logic_require_set_done(logic_require_find(t_logic_manager(), 0));
    }

    void set_require_error(void) {
        logic_require_set_error(logic_require_find(t_logic_manager(), 0));
    }
};

TEST_F(RequireRunTest, auto_commit_require_done_release) {
    LogicOpMock & op1 = installOp("Op1");
    expect_create_require(op1);

    logic_context_flag_enable(m_context, logic_context_flag_execute_immediately);

    execute("- Op1\n");
    EXPECT_EQ(logic_context_state_waiting, logic_context_state(m_context));

    expect_commit();
    set_require_done();
}

TEST_F(RequireRunTest, auto_commit_require_done_keep) {
    LogicOpMock & op1 = installOp("Op1");
    expect_create_require(op1);

    set_require_keep();
    set_execute_immediately();

    execute("- Op1\n");

    /*first execut should block*/
    EXPECT_EQ(logic_context_state_waiting, state());

    expect_commit();
    set_require_done();
}

TEST_F(RequireRunTest, auto_commit_require_error_release) {
    LogicOpMock & op1 = installOp("Op1");
    expect_create_require(op1);

    set_execute_immediately();

    execute("- Op1\n");

    EXPECT_EQ(logic_context_state_waiting, state());

    expect_commit();
    set_require_error();
}

TEST_F(RequireRunTest, auto_commit_require_error_keep) {
    LogicOpMock & op1 = installOp("Op1");

    expect_create_require(op1);

    set_require_keep();
    set_execute_immediately();

    execute("- Op1\n");
    EXPECT_EQ(logic_context_state_waiting, state());

    expect_commit();
    set_require_error();
}
