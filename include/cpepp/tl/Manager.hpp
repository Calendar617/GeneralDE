#ifndef CPEPP_TL_MANAGER_H
#define CPEPP_TL_MANAGER_H
#include "cpe/tl/tl_manage.h"
#include "System.hpp"

namespace Cpe { namespace Tl {

class Manager : public Cpe::Utils::SimulateObject {
public:
    operator tl_manage_t (void) const { return (tl_manage_t)(this); }

    int tick(int count = -1) { return tl_manage_tick(*this, count); }
};

}}

#endif
