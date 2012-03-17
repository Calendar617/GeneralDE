#include "gd/app/app_context.h"
#include "gdpp/app/Log.hpp"
#include "usf/logic/logic_context.h"
#include "usfpp/logic/LogicOpManager.hpp"

namespace Usf { namespace Logic {

LogicOpManager &
LogicOpManager::instance(gd_app_context_t app, cpe_hash_string_t name) {
    if (LogicOpManager * r = find(app, name)) {
        return *r;
    }
    else {
        if (app == 0) app = g_app_context;

        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "get Usf::Logic::LogicOpManager '%s' fail!",
            name ? cpe_hs_data(name) : "default");
    }
}

LogicOpManager &
LogicOpManager::instance(gd_app_context_t app, const char * name) {
    if (LogicOpManager * r = find(app, name)) {
        return *r;
    }
    else {
        if (app == 0) app = g_app_context;

        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "get Usf::Logic::LogicOpManager '%s' fail!",
            name ? name : "default");
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

LogicOpManager *
LogicOpManager::find(gd_app_context_t app, const char * name) {
    if (app == 0) app = g_app_context;

    logic_manage_t logic_manager =
        name == 0
        ? logic_manage_default(app)
        : logic_manage_find_nc(app, name);

    return (LogicOpManager*)logic_manager;
}

LogicOpContext &
LogicOpManager::createContext(
    size_t capacity, const void * data,
    logic_context_id_t id)
{
    logic_context_t ctx = logic_context_create(*this, id, capacity);
    if (ctx == 0) {
        APP_CTX_THROW_EXCEPTION(
            app(),
            ::std::runtime_error,
            "create Usf::Logic::LogicOpContext fail!");
    }

    if (data && logic_context_capacity(ctx) > 0) {
        memcpy(logic_context_data(ctx), data, logic_context_capacity(ctx));
    }

    return *(LogicOpContext*)ctx;
}

LogicOpManager &
LogicOpManager::install(gd_app_context_t app, mem_allocrator_t alloc, const char * name) {
    logic_manage_t manager = logic_manage_create(app, name, alloc);
    if (manager == 0) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "create Usf::Logic::LogicOpManager fail!");
    }

    return *(LogicOpManager*)manager;
}

void LogicOpManager::uninstall(gd_app_context_t app, cpe_hash_string_t name) {
    if (LogicOpManager * opManager = find(app, name)) {
        opManager->destory();
    }
}

}}
