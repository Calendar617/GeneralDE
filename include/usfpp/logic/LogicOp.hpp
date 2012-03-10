#ifndef USFPP_LOGIC_LOGICOP_H
#define USFPP_LOGIC_LOGICOP_H
#include "gdpp/nm/Object.hpp"
#include "System.hpp"

namespace Usf { namespace Logic {

class LogicOp : public Gd::Nm::Object {
public:
    typedef void (LogicOp::*execute_fun)(LogicOpContext & context, Cpe::Cfg::Node const & cfg) const;

    LogicOp(execute_fun fun);

    static LogicOp const & get(Gd::App::Application & app, cpe_hash_string_t name);
    static LogicOp const & get(Gd::App::Application & app, const char * name);

private:
    execute_fun m_exec_fun;

    static int32_t logic_op_adapter(logic_context_t ctx, void * user_data, cfg_t cfg);
};

template<typename OutT, typename ContextT>
class LogicOpDef : public LogicOp {
public:
    LogicOpDef() : LogicOp((execute_fun)&LogicOpDef::execute) {}
    ~LogicOpDef();

    virtual void execute(
        ContextT & context,
        Cpe::Cfg::Node const & cfg) const = 0;

    static OutT const & get(Gd::App::Application & app, cpe_hash_string_t name) {
        return static_cast<OutT const &>(LogicOp::get(app, name));
    }

    static OutT const & get(Gd::App::Application & app, const char * name) {
        return static_cast<OutT const &>(LogicOp::get(app, name));
    }
};

}}

#endif
