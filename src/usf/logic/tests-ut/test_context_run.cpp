#include "LogicTest.hpp"

class ContextRunTest : public LogicTest {
public:
};

TEST_F(ContextRunTest, basic_success) {
    LogicOpMock & op1 = installOp("Op1");

    EXPECT_CALL(op1, execute(::testing::_))
        .WillOnce(::testing::Return(0));

    logic_context_t context = t_logic_context_create();

    logic_executor_t executor = t_logic_executor_build("Op1");

    t_logic_execute(context, executor);

    logic_executor_free(executor);
}
