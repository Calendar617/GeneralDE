#include "gd/app/app_context.h"
#include "gdpp/app/Log.hpp"
#include "usfpp/logic/LogicOpManager.hpp"

namespace Usf { namespace Logic {

LogicOpManager &
LogicOpManager::instance(gd_app_context_t app, cpe_hash_string_t name) {
    if (LogicOpManager * r = find(app, name)) {
        return *r;
    }
    else {
        if (app == 0) app = g_app_context;

        if (name) {
            APP_CTX_THROW_EXCEPTION(
                app,
                ::std::runtime_error,
                "get Usf::Logic::LogicOpManager with name %s fail!",
                cpe_hs_data(name));
        }
        else {
            APP_CTX_THROW_EXCEPTION(
                app,
                ::std::runtime_error,
                "get Usf::Logic::LogicOpManager default fail!");
        }
    }
}

LogicOpManager *
LogicOpManager::find(gd_app_context_t app, cpe_hash_string_t name) {
    if (app == 0) app = g_app_context;

    logic_manage_t logic_manager =
        name == 0
        ? logic_manage_default(app)
        : logic_manage_find(app, name);

    return (LogicOpManager*)logic_manager;
}

}}
