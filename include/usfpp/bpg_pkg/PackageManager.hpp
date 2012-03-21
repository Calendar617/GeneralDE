#ifndef USFPP_BPG_PKG_PACKAGEMANAGER_H
#define USFPP_BPG_PKG_PACKAGEMANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "gdpp/app/Application.hpp"
#include "usf/bpg_pkg/bpg_pkg_manage.h"
#include "System.hpp"

namespace Usf { namespace Bpg {

class PackageManager : public Cpe::Utils::SimulateObject {
public:
    operator bpg_pkg_manage_t() const { return (bpg_pkg_manage_t)this; }

    Cpe::Utils::CString const & name(void) const { return Cpe::Utils::CString::_cast(bpg_pkg_manage_name(*this)); }

    Gd::App::Application & app(void) { return Gd::App::Application::_cast(bpg_pkg_manage_app(*this)); }
    Gd::App::Application const & app(void) const { return Gd::App::Application::_cast(bpg_pkg_manage_app(*this)); }
    
    bpg_pkg_t createPackage(size_t capacity, LPDRMETA carry_data_meta = NULL, size_t carry_data_capacity = NULL);

    static PackageManager & _cast(bpg_pkg_manage_t pkg_mgr);
    static PackageManager & instance(gd_app_context_t app, const char * name);
};

}}

#endif
