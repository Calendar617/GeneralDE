#ifndef CPEPP_DR_ENTRY_H
#define CPEPP_DR_ENTRY_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "System.hpp"

namespace Cpe { namespace Dr {

class Entry : public Cpe::Utils::SimulateObject {
public:
    operator LPDRMETAENTRY (void) const { return (LPDRMETAENTRY)this; }
};

}}

#endif
