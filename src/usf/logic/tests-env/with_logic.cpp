#include "gd/app/tests-env/with_app.hpp"
#include "usf/logic/tests-env/with_logic.hpp"

namespace usf { namespace logic { namespace testenv {

with_logic::with_logic() : m_logic_manager(NULL) {
}

void with_logic::SetUp() {
    m_logic_manager =
        logic_manage_create(
            0,
            envOf<gd::app::testenv::with_app>().t_app(),
            t_allocrator());
    EXPECT_TRUE(m_logic_manager) << "logic_manager create fail!";
}

void with_logic::TearDown() {
    if (m_logic_manager) {
        logic_manage_free(m_logic_manager);
        m_logic_manager = 0;
    }
}

logic_manage_t
with_logic::t_logic_manager(void) {
    EXPECT_TRUE(m_logic_manager) << "logic_manager not create!";
    return m_logic_manager;
}

}}}
