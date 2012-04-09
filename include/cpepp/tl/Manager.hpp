#ifndef CPEPP_TL_MANAGER_H
#define CPEPP_TL_MANAGER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/tl/tl_manage.h"
#include "System.hpp"

namespace Cpe { namespace Tl {

class Manager : public Cpe::Utils::SimulateObject {
public:
    operator tl_manage_t (void) const { return (tl_manage_t)(this); }

    tl_time_t curTime(void) const { return tl_manage_time(*this); }

    int tick(int count = -1) { return tl_manage_tick(*this, count); }
};

}}

#endif
