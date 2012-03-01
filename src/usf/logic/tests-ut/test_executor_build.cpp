#include "LogicTest.hpp"

class ExecutorBuildTest : public LogicTest {
public:
    using Base::t_logic_executor_build;


    static logic_executor_t build_fun(
        logic_manage_t mgr, const char * name, void * ctx,
        cfg_t args,
        error_monitor_t em)
    {
        return logic_executor_basic_create(mgr, name, NULL, NULL, args);
    }

    logic_executor_t t_logic_executor_build(const char * cfg) {
        return t_logic_executor_build(cfg, build_fun, NULL, NULL);
    }
};

TEST_F(ExecutorBuildTest, basic_basic) {
    logic_executor_t executor =
          t_logic_executor_build("Op1");

    EXPECT_STREQ(
        "Op1",
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}

TEST_F(ExecutorBuildTest, basic_with_args) {
    logic_executor_t executor =
          t_logic_executor_build("Op1: { a1: 1, a2: 2}");

    EXPECT_STREQ(
        "Op1: { a1=1, a2=2 }",
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}

TEST_F(ExecutorBuildTest, group_basic) {
    logic_executor_t executor =
        t_logic_executor_build(
            "- Op1\n"
            "- Op2: { a1: 1, a2: 2 }\n"
            );

    EXPECT_STREQ(
        "group:\n"
        "    Op1\n"
        "    Op2: { a1=1, a2=2 }"
        ,
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}

TEST_F(ExecutorBuildTest, group_group) {
    logic_executor_t executor =
        t_logic_executor_build(
            "- Op1\n"
            "-\n"
            "    - Op2: { a1: 1, a2: 2 }\n"
            );

    EXPECT_STREQ(
        "group:\n"
        "    Op1\n"
        "    group:\n"
        "        Op2: { a1=1, a2=2 }"
        ,
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}

TEST_F(ExecutorBuildTest, protected_basic) {
    logic_executor_t executor =
        t_logic_executor_build(
            "protect: Op1");

    EXPECT_STREQ(
        "protect: Op1",
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}

TEST_F(ExecutorBuildTest, protected_group) {
    logic_executor_t executor =
        t_logic_executor_build(
            "protect:\n"
            "    - Op1");

    EXPECT_STREQ(
        "protect: {\n"
        "    Op1\n"
        "}",
        t_logic_executor_dump(executor));

    logic_executor_free(executor);
}
