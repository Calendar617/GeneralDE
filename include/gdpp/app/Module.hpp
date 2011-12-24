#ifndef GDPP_APP_MODULE_H
#define GDPP_APP_MODULE_H
#include "cpepp/utils/ClassCategory.hpp"
#include "gd/app/app_module.h"
#include "app_system.hpp"

namespace Gd { namespace App {

class Module:  public Cpe::Utils::SimulateObject {
public:
    const char * name(void) const { gd_app_module_name((gd_app_module_t)this); }
};

}}

#endif

