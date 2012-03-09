#ifndef USFPP_LOGIC_LOGICOPBUILDER_H
#define USFPP_LOGIC_LOGICOPBUILDER_H
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOpBuilder {
public:
    virtual logic_executor_t build(logic_manage_t mgr, cfg_t cfg, error_monitor_t em = 0) const = 0;

    virtual ~LogicOpBuilder() throw();

    static LogicOpBuilder & instance(void);
};

}}

#endif
