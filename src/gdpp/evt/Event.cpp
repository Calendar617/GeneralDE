#include <stdexcept>
#include "gdpp/evt/Event.hpp"

namespace Gd { namespace Evt {

Event & Event::_cast(gd_evt_t evt) {
    if (evt == NULL) {
        throw ::std::runtime_error("cast to Gd::Evt::Event, input evt is null!");
    }

    return *(Event*)evt;
}

Event & Event::_cast(gd_tl_event_t tl_evt) {
    return Event::_cast(gd_evt_cvt(tl_evt));
}

}}
