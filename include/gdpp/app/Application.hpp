#ifndef GDPP_APP_MODULE_H
#define GDPP_APP_MODULE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/app/app_context.h"
#include "System.hpp"

namespace Gd { namespace App {

class Application : public Cpe::Utils::SimulateObject {
public:
    int argc(void) const { return gd_app_argc(*this); }

    mem_allocrator_t allocrator(void) { return gd_app_alloc(*this); }

    Nm::Manager & nmManager(void) { return *((Nm::Manager *)gd_app_nm_mgr(*this)); }

    operator gd_app_context_t () const { return (gd_app_context_t)(void*)this; }
};

}}

#endif

