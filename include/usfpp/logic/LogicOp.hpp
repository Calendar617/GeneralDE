#ifndef USFPP_LOGIC_LOGICOP_H
#define USFPP_LOGIC_LOGICOP_H
#include "gdpp/nm/Object.hpp"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOp : public Gd::Nm::Object {
public:
    typedef void (LogicOp::*execute_fun)(LogicOpContext & context, Cpe::Cfg::Node const & cfg) const;

    LogicOp(execute_fun fun);

    void regist_to(logic_executor_type_group_t group);

    static LogicOp & get(gd_app_context_t app, cpe_hash_string_t name);
    static LogicOp & get(gd_app_context_t app, const char * name);

private:
    execute_fun m_exec_fun;

    static int32_t logic_op_adapter(logic_context_t ctx, void * user_data, cfg_t cfg);
};

template<typename OutT, typename ContextT>
class LogicOpDef : public LogicOp {
public:
    LogicOpDef() : LogicOp((execute_fun)&LogicOpDef::execute) {}

    virtual void execute(
        ContextT & context,
        Cpe::Cfg::Node const & cfg) const = 0;
};

}}

#endif
