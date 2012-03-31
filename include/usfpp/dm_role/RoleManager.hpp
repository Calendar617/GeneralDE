#ifndef USFPP_DM_ROLEMANAGER_H
#define USFPP_DM_ROLEMANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/dm_role/dm_role_manage.h"
#include "System.hpp"

namespace Usf { namespace Dm {

class RoleManager : public Cpe::Utils::SimulateObject {
public:
    operator dm_role_manage_t() const { return (dm_role_manage_t)this; }

    Cpe::Utils::CString const & name(void) const { return Cpe::Utils::CString::_cast(dm_role_manage_name(*this)); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(dm_role_manage_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(dm_role_manage_app(*this)); }

    static RoleManager & _cast(dm_role_manage_t role_manage);
    static RoleManager & instance(gd_app_context_t app, const char * name = NULL);
};

}}

#endif
