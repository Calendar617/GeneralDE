#include "usf/logic/logic_require.h"
#include "RunTest.hpp"

class ContextRunTest : public RunTest {
};

TEST_F(ContextRunTest, basic_success) {
    LogicOpMock & op1 = installOp("Op1");
    expect_return(op1, 0);

    expect_commit();
    execute("Op1");

    EXPECT_EQ(logic_context_state_done, state());
    EXPECT_EQ((int32_t)0, rv());
}

TEST_F(ContextRunTest, basic_fail) {
    LogicOpMock & op1 = installOp("Op1");
    expect_return(op1, 123);

    expect_commit();
    execute("Op1");

    EXPECT_EQ(logic_context_state_error, state());
    EXPECT_EQ((int32_t)123, rv());
}

TEST_F(ContextRunTest, group_basic) {
    LogicOpMock & op1 = installOp("Op1");
    expect_return(op1, 0);

    expect_commit();
    execute("- Op1");

    EXPECT_EQ(logic_context_state_done, state());
    EXPECT_EQ((int32_t)0, rv());
}

TEST_F(ContextRunTest, group_multi) {
    LogicOpMock & op1 = installOp("Op1");
    LogicOpMock & op2 = installOp("Op2");

    do {
        ::testing::InSequence s;
        expect_return(op1, 0);
        expect_return(op2, 0);
    } while(0);

    expect_commit();
    execute(
        "- Op1\n"
        "- Op2"
        );

    EXPECT_EQ(logic_context_state_done, state());
    EXPECT_EQ((int32_t)0, rv());
}

TEST_F(ContextRunTest, group_multi_error_break) {
    LogicOpMock & op1 = installOp("Op1");
    expect_return(op1, 123);
    installOp("Op2");

    expect_commit();
    execute(
        "- Op1\n"
        "- Op2"
        );

    EXPECT_EQ(logic_context_state_error, state());
    EXPECT_EQ((int32_t)123, rv());
}

TEST_F(ContextRunTest, group_multi_level_error_break) {
    LogicOpMock & op1 = installOp("Op1");
    installOp("Op2");
    installOp("Op3");
    installOp("Op4");

    expect_return(op1, 123);

    expect_commit();
    execute(
        "-\n"
        "  -\n"
        "    - Op1\n"
        "    - Op2\n"
        "  - Op3\n"
        "- Op4"
        );

    EXPECT_EQ(logic_context_state_error, state());
    EXPECT_EQ((int32_t)123, rv());
}

TEST_F(ContextRunTest, protected_basic) {
    LogicOpMock & op1 = installOp("Op1");
    expect_return(op1, 123);

    expect_commit();
    execute("protect: Op1");

    EXPECT_EQ(logic_context_state_done, state());
    EXPECT_EQ((int32_t)0, rv());
}

TEST_F(ContextRunTest, waiting_basic) {
    LogicOpMock & op1 = installOp("Op1");
    LogicOpMock & op2 = installOp("Op2");

    expect_create_require(op1);

    execute(
        "- Op1\n"
        "- Op2\n"
        );

    EXPECT_EQ(logic_context_state_waiting, state());
    EXPECT_EQ((int32_t)0, rv());

    logic_require_free(logic_require_find(t_logic_manage(), 0));

    expect_return(op2, 123);
    expect_commit();
    execute_again();
}

TEST_F(ContextRunTest, waiting_no_left_op) {
    LogicOpMock & op1 = installOp("Op1");

    expect_create_require(op1);

    execute("- Op1\n");

    /*first execut should block*/
    EXPECT_EQ(logic_context_state_waiting, state());
    EXPECT_EQ((int32_t)0, rv());

    logic_require_free(logic_require_find(t_logic_manage(), 0));

    /*retry*/
    expect_commit();
    execute_again();
}

TEST_F(ContextRunTest, cancel_in_wait) {
    LogicOpMock & op1 = installOp("Op1");
    installOp("Op2");

    expect_create_require(op1);

    execute(
        "- Op1\n"
        "- Op2\n"
        );

    /*first execut should block*/
    EXPECT_EQ(logic_context_state_waiting, state());
    EXPECT_EQ((int32_t)0, rv());

    expect_commit();
    cancel();

    EXPECT_EQ(logic_context_state_cancel, state());
}

TEST_F(ContextRunTest, timeout_in_wait) {
    LogicOpMock & op1 = installOp("Op1");
    installOp("Op2");

    expect_create_require(op1);

    execute(
        "- Op1\n"
        "- Op2\n"
        );

    /*first execut should block*/
    EXPECT_EQ(logic_context_state_waiting, state());
    EXPECT_EQ((int32_t)0, rv());

    expect_commit();
    timeout();

    EXPECT_EQ(logic_context_state_timeout, state());
}
