#ifndef GDPP_APP_MODULE_H
#define GDPP_APP_MODULE_H
#include "gd/app/app_context.h"
#include "System.hpp"

namespace Gd { namespace App {

class Application {
public:
    int argc(void) const { return gd_app_argc((gd_app_context_t)this); }

    Nm::Manager & nmManager(void) { return *((Nm::Manager *)(void*)gd_app_nm_mgr((gd_app_context_t)this)); }

private:
    Application();
    ~Application();
};

}}

#endif

