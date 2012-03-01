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

private:
    logic_manage_t m_logic_manager;
};

}}}

#endif
