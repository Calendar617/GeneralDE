#ifndef CPEPP_DR_META_H
#define CPEPP_DR_META_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class Meta : public Cpe::Utils::SimulateObject {
public:
    operator LPDRMETA (void) const { return (LPDRMETA)this; }

    static Meta const & _cast(LPDRMETA meta);
};

}}

#endif
