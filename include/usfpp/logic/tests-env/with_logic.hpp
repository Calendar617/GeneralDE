#ifndef USFPP_LOGIC_TESTENV_WITH_LOGIC_H
#define USFPP_LOGIC_TESTENV_WITH_LOGIC_H
#include "usf/logic/tests-env/with_logic.hpp"
#include "../LogicOpManager.hpp"

namespace Usf { namespace Logic { namespace testenv {

class with_logic : public usf::logic::testenv::with_logic {
    typedef usf::logic::testenv::with_logic Base;
public:
    with_logic();

    void SetUp();
    void TearDown();

    using Base::t_logic_executor_build;
    logic_executor_t t_logic_executor_build(cfg_t cfg, error_monitor_t em = 0);
    logic_executor_t t_logic_executor_build(const char * cfg, error_monitor_t em = 0);

    LogicOpManager & t_logic_manager_ex(void) { return *(LogicOpManager*)t_logic_manager(); }
};

}}}

#endif
