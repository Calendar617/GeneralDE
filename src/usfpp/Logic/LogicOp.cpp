#include "gdpp/nm/Manager.hpp"
#include "gdpp/app/Log.hpp"
#include "gdpp/app/Application.hpp"
#include "usfpp/logic/LogicOp.hpp"

namespace Usf { namespace Logic {

LogicOp::~LogicOp() {
}

LogicOp const &
LogicOp::get(Gd::App::Application & app, cpe_hash_string_t name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(
            &app.nmManager().object(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "RoleManage cast fail!");
    }
    return *r;
}

LogicOp const &
LogicOp::get(Gd::App::Application & app, const char * name) {
    LogicOp * r =
        dynamic_cast<LogicOp *>(&app.nmManager().objectNc(name));
    if (r == NULL) {
        APP_CTX_THROW_EXCEPTION(app, ::std::runtime_error, "RoleManage cast fail!");
    }
    return *r;
}

}}
