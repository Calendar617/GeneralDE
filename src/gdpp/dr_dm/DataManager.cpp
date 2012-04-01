#include "gdpp/app/Log.hpp"
#include "gdpp/dr_dm/DataManager.hpp"

namespace Gd { namespace Dr {

DataManager & DataManager::_cast(dr_dm_manage_t role_manage) {
    if (role_manage == NULL) {
        throw ::std::runtime_error("Gd::Dr::DataManager::_cast: input role_manage is NULL!");
    }

    return *(DataManager*)role_manage;
}

DataManager & DataManager::instance(gd_app_context_t app, const char * name) {
    dr_dm_manage_t role_manage = dr_dm_manage_find_nc(app, name);
    if (role_manage == NULL) {
        APP_CTX_THROW_EXCEPTION(
            app,
            ::std::runtime_error,
            "role_manage %s not exist!", name ? name : "default");
    }

    return *(DataManager*)role_manage;
}

}}
