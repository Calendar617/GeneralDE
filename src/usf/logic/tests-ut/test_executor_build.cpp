#include "LogicTest.hpp"

class ExecutorBuildTest : public LogicTest {
};

TEST_F(ExecutorBuildTest, basic_basic) {
    installLogicOpMock("Op1");

    ::std::auto_ptr<LogicOpPlain> plain =
          createLogicOpPlain("Op1");

    EXPECT_STREQ(
        "Op1",
        dumpLogicOpPlain(*plain));
}

TEST_F(ExecutorBuildTest, basic_with_args) {
    installLogicOpMock("Op1");

    ::std::auto_ptr<LogicOpPlain> plain =
          createLogicOpPlain("Op1: { a1: 1, a2: 2}");

    EXPECT_STREQ(
        "Op1: { a1=1, a2=2 }",
        dumpLogicOpPlain(*plain));
}

TEST_F(ExecutorBuildTest, group_basic) {
    installLogicOpMock("Op1");
    installLogicOpMock("Op2");

    ::std::auto_ptr<LogicOpPlain> plain =
        createLogicOpPlain(
            "- Op1\n"
            "- Op2: { a1: 1, a2: 2 }\n"
            );

    EXPECT_STREQ(
        "{\n"
        "    Op1\n"
        "    Op2: { a1=1, a2=2 }\n"
        "}",
        dumpLogicOpPlain(*plain));
}

TEST_F(ExecutorBuildTest, group_group) {
    installLogicOpMock("Op1");
    installLogicOpMock("Op2");

    ::std::auto_ptr<LogicOpPlain> plain =
        createLogicOpPlain(
            "- Op1\n"
            "-\n"
            "    - Op2: { a1: 1, a2: 2 }\n"
            );

    EXPECT_STREQ(
        "{\n"
        "    Op1\n"
        "    {\n"
        "        Op2: { a1=1, a2=2 }\n"
        "    }\n"
        "}",
        dumpLogicOpPlain(*plain));
}

TEST_F(ExecutorBuildTest, protected_basic) {
    installLogicOpMock("Op1");

    ::std::auto_ptr<LogicOpPlain> plain =
        createLogicOpPlain(
            "protect: Op1");

    EXPECT_STREQ(
        "protect: Op1",
        dumpLogicOpPlain(*plain));
}

TEST_F(ExecutorBuildTest, protected_group) {
    installLogicOpMock("Op1");

    ::std::auto_ptr<LogicOpPlain> plain =
        createLogicOpPlain(
            "protect:\n"
            "    - Op1");

    EXPECT_STREQ(
        "protect: {\n"
        "    Op1\n"
        "}",
        dumpLogicOpPlain(*plain));
}
