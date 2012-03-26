#include <stdexcept>
#include "cpe/dr/dr_json.h"
#include "cpe/tl/tl_action.h"
#include "gdpp/evt/Event.hpp"

namespace Gd { namespace Evt {

Event & Event::_cast(gd_evt_t evt) {
    if (evt == NULL) {
        throw ::std::runtime_error("cast to Gd::Evt::Event, input evt is null!");
    }

    return *(Event*)evt;
}

Event & Event::_cast(tl_event_t tl_evt) {
    return Event::_cast(gd_evt_cvt(tl_evt));
}

Event * Event::clone(mem_allocrator_t alloc) const {
    tl_event_t from_evt = tl_event_from_data((void*)(gd_evt_t)*this);

    tl_event_t new_evt = tl_event_clone(from_evt, alloc);
    if (new_evt == NULL) {
        throw ::std::runtime_error("clone Gd::Evt::Event, return evt is null!");
    }

    return (Event*)gd_evt_cvt(new_evt);
}

void Event::destory(void) {
    tl_event_t tl_evt = tl_event_from_data((void*)(gd_evt_t)this);
    tl_event_free(tl_evt);
}

void Event::dump(write_stream_t stream) const {
    dr_json_print(stream, gd_evt_data(*this), gd_evt_meta(*this), DR_JSON_PRINT_MINIMIZE, NULL);
}

}}
