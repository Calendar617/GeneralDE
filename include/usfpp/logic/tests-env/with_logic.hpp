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

    LogicOpManager & t_logic_manager_ex(void) { return *(LogicOpManager*)t_logic_manager(); }
};

}}}

#endif
