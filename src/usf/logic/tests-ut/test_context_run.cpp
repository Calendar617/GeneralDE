#include "usf/logic/logic_require.h"
#include "usf/logic/logic_require_type.h"
#include "LogicTest.hpp"

class ContextRunTest : public LogicTest {
public:
    virtual void SetUp() {
        LogicTest::SetUp();
        logic_require_type_create(t_logic_manager(), "r1");
    }

    static void create_rquire(logic_context_t context) {
        char name[] = CPE_HS_BUF_MAKE("r1");
        logic_require_create(context, (cpe_hash_string_t)name, 0);
    }
};

TEST_F(ContextRunTest, basic_success) {
    LogicOpMock & op1 = installOp("Op1");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(0));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor = t_logic_executor_build("Op1");

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_done, logic_context_state(context));
    EXPECT_EQ((int32_t)0, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, basic_fail) {
    LogicOpMock & op1 = installOp("Op1");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(123));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor = t_logic_executor_build("Op1");

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_error, logic_context_state(context));
    EXPECT_EQ((int32_t)123, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, group_basic) {
    LogicOpMock & op1 = installOp("Op1");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(0));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor = t_logic_executor_build("- Op1");

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_done, logic_context_state(context));
    EXPECT_EQ((int32_t)0, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, group_multi) {
    LogicOpMock & op1 = installOp("Op1");
    LogicOpMock & op2 = installOp("Op2");

    do {
        ::testing::InSequence s;

        EXPECT_CALL(op1, execute(::testing::_))
            .WillOnce(::testing::Return(0));

        EXPECT_CALL(op2, execute(::testing::_))
            .WillOnce(::testing::Return(0));
    } while(0);

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor =
        t_logic_executor_build(
            "- Op1\n"
            "- Op2"
            );

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_done, logic_context_state(context));
    EXPECT_EQ((int32_t)0, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, group_multi_error_break) {
    LogicOpMock & op1 = installOp("Op1");
    installOp("Op2");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(123));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor =
        t_logic_executor_build(
            "- Op1\n"
            "- Op2"
            );

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_error, logic_context_state(context));
    EXPECT_EQ((int32_t)123, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, group_multi_level_error_break) {
    LogicOpMock & op1 = installOp("Op1");
    installOp("Op2");
    installOp("Op3");
    installOp("Op4");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(123));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor =
        t_logic_executor_build(
            "-\n"
            "  -\n"
            "    - Op1\n"
            "    - Op2\n"
            "  - Op3\n"
            "- Op4"
            );

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_error, logic_context_state(context));
    EXPECT_EQ((int32_t)123, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, protected_basic) {
    LogicOpMock & op1 = installOp("Op1");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(123));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor = t_logic_executor_build("protect: Op1");

    t_logic_execute(context, executor);

    EXPECT_EQ(logic_context_state_done, logic_context_state(context));
    EXPECT_EQ((int32_t)0, logic_context_errno(context));

    logic_executor_free(executor);
}

TEST_F(ContextRunTest, waiting_basic) {
    LogicOpMock & op1 = installOp("Op1");
    LogicOpMock & op2 = installOp("Op2");

    CommitMock commitMock;

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(
            ::testing::DoAll(
                ::testing::Invoke(create_rquire),
                ::testing::Return(0)));

    logic_context_t context = t_logic_context_create();
    set_commit(context, commitMock);

    logic_executor_t executor =
        t_logic_executor_build(
            "- Op1\n"
            "- Op2\n"
            );

    /*first execut should block*/
    t_logic_execute(context, executor);
    EXPECT_EQ(logic_context_state_waiting, logic_context_state(context));
    EXPECT_EQ((int32_t)0, logic_context_errno(context));

    logic_require_free(logic_require_find(t_logic_manager(), 0));

    /*retry*/
    EXPECT_CALL(op2, execute(::testing::_))
        .WillOnce(::testing::Return(123));
    EXPECT_CALL(commitMock, commit(::testing::_));
    logic_context_execute(context);

    logic_executor_free(executor);
}
