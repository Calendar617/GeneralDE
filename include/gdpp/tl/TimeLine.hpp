#ifndef GDPP_TL_TIMELINE_H
#define GDPP_TL_TIMELINE_H
#include "gd/tl/tl_manage.h"
#include "System.hpp"

namespace Gd { namespace Tl {

class TimeLine : public Cpe::Utils::SimulateObject {
public:
    operator gd_tl_t (void) const { return (gd_tl_t)(this); }
};

}}

#endif
