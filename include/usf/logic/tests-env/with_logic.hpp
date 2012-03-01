#ifndef USF_LOGIC_TESTENV_WITH_LOGIC_H
#define USF_LOGIC_TESTENV_WITH_LOGIC_H
#include "../logic_manage.h"
#include "../logic_executor.h"
#include "../logic_context.h"
#include "cpe/utils/tests-env/test-env.hpp"

namespace usf { namespace logic { namespace testenv {

class with_logic : public ::testenv::env<> {
public:
    with_logic();

    void SetUp();
    void TearDown();

    logic_manage_t t_logic_manager(void);

    logic_executor_t t_logic_executor_build(cfg_t cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em = 0);
    logic_executor_t t_logic_executor_build(const char * cfg, logic_executor_build_fun_t fun, void * ctx, error_monitor_t em = 0);

    const char * t_logic_executor_dump(logic_executor_t executor);

private:
    logic_manage_t m_logic_manager;
};

}}}

#endif
