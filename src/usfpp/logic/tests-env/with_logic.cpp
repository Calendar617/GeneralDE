#include "usfpp/logic/LogicOp.hpp"
#include "usfpp/logic/tests-env/with_logic.hpp"

namespace Usf { namespace Logic { namespace testenv {

with_logic::with_logic() {
}

void with_logic::SetUp() {
    Base::SetUp();
}

void with_logic::TearDown() {
    Base::TearDown();
}

logic_executor_t
with_logic::t_logic_executor_build(cfg_t cfg, error_monitor_t em) {
    return t_logic_executor_build(cfg, LogicOp::builder, 0, em);
}

logic_executor_t
with_logic::t_logic_executor_build(const char * cfg, error_monitor_t em) {
    return t_logic_executor_build(cfg, LogicOp::builder, 0, em);
}

}}}
