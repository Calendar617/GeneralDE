#include "cpepp/cfg/Node.hpp"
#include "gdpp/nm/Manager.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/logic/logic_executor.h"
#include "usf/logic/logic_executor_type.h"
#include "usf/logic/logic_manage.h"
#include "usf/logic/logic_context.h"
#include "usfpp/logic/LogicOp.hpp"

namespace Usf { namespace Logic {

LogicOp::LogicOp(execute_fun fun) : m_exec_fun(fun) {
}

LogicOp &
LogicOp::get(gd_app_context_t app, cpe_hash_string_t name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(
            &Gd::App::Application::_cast(app).nmManager().object(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "LogicOp %s cast fail!", cpe_hs_data(name));
    }
    return *r;
}

LogicOp &
LogicOp::get(gd_app_context_t app, const char * name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(
            &Gd::App::Application::_cast(app).nmManager().objectNc(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "LogicOp %s cast fail!", name);
    }
    return *r;
}

void LogicOp::regist_to(logic_executor_type_group_t group) {
    logic_executor_type_t type = logic_executor_type_create(group, name(), logic_executor_category_basic);
    if (type == NULL) {
        APP_CTX_THROW_EXCEPTION(
            logic_executor_type_group_app(group),
            ::std::runtime_error,
            "logic op type %s already exist in group %s!",
            name(), logic_executor_type_group_name(group));
    }

    logic_executor_type_bind_basic(type, logic_op_adapter, this);
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

}}
