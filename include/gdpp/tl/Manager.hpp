#ifndef GDPP_TL_MANAGER_H
#define GDPP_TL_MANAGER_H
#include "gd/tl/tl_manage.h"
#include "System.hpp"

namespace Gd { namespace Tl {

class Manager : public Cpe::Utils::SimulateObject {
public:
    operator gd_tl_manage_t (void) const { return (gd_tl_manage_t)(this); }

    
};

}}

#endif
