#ifndef CPEPP_DR_METALIB_H
#define CPEPP_DR_METALIB_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class MetaLib : public Cpe::Utils::SimulateObject {
public:
    operator LPDRMETALIB (void) const { return (LPDRMETALIB)this; }
};

}}

#endif
