#ifndef GDPP_APP_APPLICATION_H
#define GDPP_APP_APPLICATION_H
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

    Cpe::Cfg::Node & cfg(void) { return *(Cpe::Cfg::Node *)gd_app_cfg(*this); }
    Cpe::Cfg::Node const & cfg(void) const { return *(Cpe::Cfg::Node *)gd_app_cfg(*this); }

    Cpe::Dp::Manager & dpManager(void) { return *((Cpe::Dp::Manager *)gd_app_dp_mgr(*this)); }
    Cpe::Dp::Manager const & dpManager(void) const { return *((Cpe::Dp::Manager *)gd_app_dp_mgr(*this)); }

    Cpe::Nm::Manager & nmManager(void) { return *((Cpe::Nm::Manager *)gd_app_nm_mgr(*this)); }
    Cpe::Nm::Manager const & nmManager(void) const { return *((Cpe::Nm::Manager *)gd_app_nm_mgr(*this)); }

    Cpe::Tl::Manager & tlManager(void) { return *(Cpe::Tl::Manager *)gd_app_tl_mgr(*this); }
    Cpe::Tl::Manager const & tlManager(void) const { return *(Cpe::Tl::Manager *)gd_app_tl_mgr(*this); }

    void tick(void) { gd_app_tick(*this); }

    static Application & instance(void);
    static Application & _cast(gd_app_context_t ctx);
};

}}

#endif

