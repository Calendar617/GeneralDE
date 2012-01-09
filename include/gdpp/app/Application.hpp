#ifndef GDPP_APP_MODULE_H
#define GDPP_APP_MODULE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/app/app_context.h"
#include "System.hpp"

namespace Gd { namespace App {

class Application : public Cpe::Utils::SimulateObject {
public:
    operator gd_app_context_t () const { return (gd_app_context_t)(void*)this; }

    int argc(void) const { return gd_app_argc(*this); }
    char ** argv(void) const { return gd_app_argv(*this); }

    mem_allocrator_t allocrator(void) { return gd_app_alloc(*this); }

    Dp::Manager & dpManager(void) { return *((Dp::Manager *)gd_app_dp_mgr(*this)); }
    Dp::Manager const & dpManager(void) const { return *((Dp::Manager *)gd_app_dp_mgr(*this)); }

    Nm::Manager & nmManager(void) { return *((Nm::Manager *)gd_app_nm_mgr(*this)); }
    Nm::Manager const & nmManager(void) const { return *((Nm::Manager *)gd_app_nm_mgr(*this)); }

    Tl::Manager & tlManager(void) { return *(Tl::Manager *)gd_app_tl_mgr(*this); }
    Tl::Manager const & tlManager(void) const { return *(Tl::Manager *)gd_app_tl_mgr(*this); }

    static Application & instance(void);
    static Application & _cast(gd_app_context_t ctx);
};

}}

#endif

