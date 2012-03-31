#include "gdpp/app/Log.hpp"
#include "usfpp/dm_role/RoleManager.hpp"

namespace Usf { namespace Dm {

RoleManager & RoleManager::_cast(dm_role_manage_t role_manage) {
    if (role_manage == NULL) {
        throw ::std::runtime_error("Usf::Dm::RoleManager::_cast: input role_manage is NULL!");
    }

    return *(RoleManager*)role_manage;
}

RoleManager & RoleManager::instance(gd_app_context_t app, const char * name) {
    dm_role_manage_t role_manage = dm_role_manage_find_nc(app, name);
    if (role_manage == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "role_manage %s not exist!", name ? name : "default");
    }

    return *(RoleManager*)role_manage;
}

}}
