#include "cpepp/cfg/Node.hpp"
#include "gdpp/nm/Manager.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/logic/logic_executor.h"
#include "usf/logic/logic_manage.h"
#include "usf/logic/logic_context.h"
#include "usfpp/logic/LogicOp.hpp"

namespace Usf { namespace Logic {

LogicOp::LogicOp(execute_fun fun) : m_exec_fun(fun) {
}

LogicOp const &
LogicOp::get(Gd::App::Application & app, cpe_hash_string_t name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(
            &app.nmManager().object(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "LogicOp %s cast fail!", cpe_hs_data(name));
    }
    return *r;
}

LogicOp const &
LogicOp::get(Gd::App::Application & app, const char * name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(
            &app.nmManager().objectNc(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "LogicOp %s cast fail!", name);
    }
    return *r;
}

int32_t LogicOp::logic_op_adapter(logic_context_t ctx, void * user_data, cfg_t cfg) {
    LogicOp * op = (LogicOp*)user_data;
    try {
        (op->*(op->m_exec_fun))(*(LogicOpContext*)ctx, Cpe::Cfg::Node::_cast(cfg));
        return 0;
    }
    APP_CTX_CATCH_EXCEPTION(logic_context_app(ctx), "execute logic op: ");

    return -1;
}

logic_executor_t
LogicOp::create_executor(
    logic_manage_t mgr,
    LogicOp const & op,
    cfg_t args)
{
    logic_executor_t executor =
        logic_executor_basic_create(
            mgr,
            op.name(),
            logic_op_adapter,
            (void *)&op,
            args);

    if (executor == 0) {
        APP_CTX_THROW_EXCEPTION(
            logic_manage_app(mgr),
            ::std::runtime_error,
            "create logic_executor from LogicOp fail!");
    }

    return executor;
}

logic_executor_t
LogicOp::builder(logic_manage_t mgr, const char * name, void * ctx, cfg_t args, error_monitor_t em) {
    try {
        return create_executor(
            mgr,
            get(Gd::App::Application::_cast(logic_manage_app(mgr)), name),
            args);
    }
    catch(::std::exception const & e) {
        CPE_ERROR(em, "create logic_executor %s: %s", name, e.what());
    }
    catch(...) {
        CPE_ERROR(em, "create logic_executor %s: unknown exception", name);
    }

    return 0;
}

}}
