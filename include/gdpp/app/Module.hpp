#ifndef GDPP_APP_MODULE_H
#define GDPP_APP_MODULE_H
#include "gd/app/app_module.h"
#include "app_system.hpp"

namespace Gd { namespace App {

class Module {
public:
    const char * name(void) const { gd_app_module_name((gd_app_module_t)this); }

private:
    Module();
    ~Module();
};

}}

#endif

