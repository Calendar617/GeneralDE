#ifndef USFPP_DM_ROLEMANAGERGEN_H
#define USFPP_DM_ROLEMANAGERGEN_H
#include "RoleManager.hpp"

namespace Usf { namespace Dm {

template<typename ManagerT, typename RoleT>
class RoleManagerGen : public RoleManager {
public:
    static ManagerT & _cast(dm_role_manage_t role_manage) { 
        return static_cast<ManagerT &>(RoleManager::_cast(role_manage));
    }

    static ManagerT & instance(gd_app_context_t app, const char * name = NULL) {
        return static_cast<ManagerT &>(RoleManager::instance(app, name));
    }
};

}}

#endif
