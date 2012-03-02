#include <stdexcept>
#include "LogicTest.hpp"

void LogicTest::SetUp() {
    Base::SetUp();
}

void LogicTest::TearDown() {
    for(LogicOpContainer::iterator it = m_ops.begin(); it != m_ops.end(); ++it) {
        delete it->second;
    }
    m_ops.clear();

    Base::TearDown();
}

static int32_t execute_fun (logic_context_t ctx, void * user_data, cfg_t cfg) {
    LogicTest::LogicOpMock * op = (LogicTest::LogicOpMock *)user_data;
    return op->execute(ctx);
}

static logic_executor_t build_fun(
    logic_manage_t mgr, const char * name, void * ctx,
    cfg_t args,
    error_monitor_t em)
{
    LogicTest::LogicOpContainer * ops = (LogicTest::LogicOpContainer *)ctx;

    LogicTest::LogicOpContainer::iterator pos = ops->find(name);
    if (pos == ops->end()) return NULL;
    
    return logic_executor_basic_create(mgr, name, execute_fun, pos->second, args);
}

logic_executor_t
LogicTest::t_logic_executor_build(const char * cfg) {
    return t_logic_executor_build(cfg, build_fun, &m_ops, NULL);
}

LogicTest::LogicOpMock &
LogicTest::installOp(const char * name) {
    LogicOpContainer::iterator pos = m_ops.find(name);

    EXPECT_TRUE(pos == m_ops.end()) << "logic op " << name << " already exist!";
    if (pos != m_ops.end()) return *pos->second;

    pos = m_ops.insert(LogicOpContainer::value_type(name, new LogicOpMock)).first;

    return *pos->second;
}

LogicTest::LogicOpMock &
LogicTest::op(const char * name) {
    LogicOpContainer::iterator pos = m_ops.find(name);

    EXPECT_TRUE(pos != m_ops.end()) << "logic op " << name << " not exist!";
    
    if (pos != m_ops.end()) {
        throw ::std::runtime_error("logic op not exist!");
    }
    else {
        return *pos->second;
    }
}
