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

    Event & createEvent(const char * name);
};

}}

#endif
