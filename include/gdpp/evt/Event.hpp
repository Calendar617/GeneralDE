#ifndef GDPP_NM_OBJECT_H
#define GDPP_NM_OBJECT_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/DataProxy.hpp"
#include "gd/evt/evt_read.h"
#include "System.hpp"

namespace Gd { namespace Evt {

class Event : public Cpe::Utils::SimulateObject {
public:
    operator gd_evt_t (void) const { return (gd_evt_t)this; }

    const char * type(void) const { return gd_evt_type(*this); }

    Cpe::Dr::Meta const & meta(void) const {
        return Cpe::Dr::Meta::_cast(gd_evt_meta(*this));
    }

    Cpe::Dr::ConstData args(void) const { 
        return Cpe::Dr::ConstData(gd_evt_data(*this), meta());
    }

    Cpe::Dr::Data args(void) {
        return Cpe::Dr::ConstData(gd_evt_data(*this), meta());
    }
};

}}

#endif
