#include <stdexcept>
#include "usf/logic/logic_require_type.h"
#include "usf/logic/logic_executor_type.h"
#include "LogicTest.hpp"

void LogicTest::SetUp() {
    Base::SetUp();
    t_em_set_print();
}

void LogicTest::TearDown() {
    logic_executor_type_it it;
    logic_executor_type_group_types(&it, t_logic_executor_type_group(NULL));

    while(logic_executor_type_t type = logic_executor_type_next(&it)) {
        delete (LogicTest::LogicOpMock*)logic_executor_type_ctx(type);
        logic_executor_type_bind_basic(type, NULL, NULL);
    }

    Base::TearDown();
}

static int32_t execute_fun (logic_context_t ctx, logic_executor_t executor, void * user_data, cfg_t cfg) {
    LogicTest::LogicOpMock * op = (LogicTest::LogicOpMock *)user_data;
    return op->execute(ctx);
}

LogicTest::LogicOpMock &
LogicTest::installOp(const char * name) {
    logic_executor_type_group_t group = t_logic_executor_type_group(NULL);

    logic_executor_type_t type = logic_executor_type_find(group, name);
    EXPECT_TRUE(type == NULL) << "logic op " << name << " already exist!";
    if (type) {
        return *(LogicTest::LogicOpMock*)logic_executor_type_ctx(type);
    }

    type = logic_executor_type_create(group, name, logic_executor_category_basic);
    EXPECT_TRUE(type != NULL) << "logic op " << name << " create fail";
    if (type == NULL) {
        throw ::std::runtime_error("logic op not exist!");
    }

    EXPECT_EQ(0, logic_executor_type_bind_basic(type, execute_fun, new LogicOpMock));

    return *(LogicTest::LogicOpMock*)logic_executor_type_ctx(type);
}

LogicTest::LogicOpMock &
LogicTest::op(const char * name) {
    logic_executor_type_group_t group = t_logic_executor_type_group(NULL);

    logic_executor_type_t type = logic_executor_type_find(group, name);
    EXPECT_TRUE(type) << "logic op " << name << " not exist!";
    if (type == NULL) {
        throw ::std::runtime_error("logic op not exist!");
    }

    return *(LogicTest::LogicOpMock*)logic_executor_type_ctx(type);
}

static void commit_to_mock(logic_context_t ctx, void * user_data) {
    ((LogicTest::CommitMock *)user_data)->commit(ctx);
}

void LogicTest::set_commit(logic_context_t context, CommitMock & mock) {
    logic_context_set_commit(context, commit_to_mock, &mock);
}

static void rt_destory_to_mock(logic_require_t require, void * user_data) {
    ((LogicTest::RequireTypeMock*)user_data)->destory(require);
}

void LogicTest::set_destory(logic_require_type_t rt, RequireTypeMock & mock) {
    logic_require_type_set_destory(rt, rt_destory_to_mock, &mock);
}

static void rt_cancel_to_mock(logic_require_t require, void * user_data) {
    ((LogicTest::RequireTypeMock*)user_data)->cancel(require);
}

void LogicTest::set_cancel(logic_require_type_t rt, RequireTypeMock & mock) {
    logic_require_type_set_cancel(rt, rt_cancel_to_mock, &mock);
}
