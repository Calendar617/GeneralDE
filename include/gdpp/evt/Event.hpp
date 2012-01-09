#ifndef GDPP_EVT_EVENT_H
#define GDPP_EVT_EVENT_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/utils/CString.hpp"
#include "cpepp/dr/Meta.hpp"
#include "cpepp/dr/Data.hpp"
#include "gd/evt/evt_read.h"
#include "System.hpp"

namespace Gd { namespace Evt {

class Event : public Cpe::Utils::SimulateObject {
public:
    operator gd_evt_t (void) const { return (gd_evt_t)this; }

    Cpe::Utils::CString const & type(void) const { return Cpe::Utils::CString::_cast(gd_evt_type(*this)); }

    Cpe::Dr::Meta const & meta(void) const {
        return Cpe::Dr::Meta::_cast(gd_evt_meta(*this));
    }

    void * attach_buf(void) { return gd_evt_attach(*this); }
    size_t attach_capacity(void) { return gd_evt_attach_capacity(*this); }

    Cpe::Dr::ConstData args(void) const { 
        return Cpe::Dr::ConstData(gd_evt_data(*this), meta());
    }

    Cpe::Dr::Data args(void) {
        return Cpe::Dr::Data(gd_evt_data(*this), meta());
    }

    Cpe::Dr::ConstDataElement operator[] (const char * name) const {
        return args()[name];
    }

    Cpe::Dr::DataElement operator[] (const char * name) {
        return args()[name];
    }

    static Event & _cast(gd_evt_t evt);
    static Event & _cast(gd_tl_event_t tl_evt);
};

}}

#endif
