#ifndef GDPP_EVT_EVENTCENTER_H
#define GDPP_EVT_EVENTCENTER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gd/evt/evt_manage.h"
#include "System.hpp"

namespace Gd { namespace Evt {

class EventCenter : public Cpe::Utils::SimulateObject {
public:
    operator gd_evt_mgr_t (void) const { return (gd_evt_mgr_t)this; }

    Cpe::Dr::MetaLib const & metaLib(void) const { return *(Cpe::Dr::MetaLib*)gd_evt_mgr_metalib(*this); }

    Event & createEvent(size_t attach_capacity, const char * name, ssize_t data_capacity = -1);
    void sendEvent(Event & event);

    static EventCenter & _cast(gd_evt_mgr_t evm);
};

}}

#endif
