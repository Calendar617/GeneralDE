#include "usf/logic/logic_build.h"
#include "usfpp/logic/LogicOpBuilder.hpp"
#include "usfpp/logic/LogicOp.hpp"

namespace Usf { namespace Logic {

class LogicOpBuilderImpl : public LogicOpBuilder {
public:
    logic_executor_t build(logic_manage_t mgr, cfg_t cfg, error_monitor_t em) const {
        return logic_executor_build(mgr, cfg, LogicOp::builder, NULL, em);
    }

};

LogicOpBuilder::~LogicOpBuilder() throw() {
}

LogicOpBuilder &
LogicOpBuilder::instance(void) {
    static LogicOpBuilderImpl s_ins;
    return s_ins;
}
 
}}
