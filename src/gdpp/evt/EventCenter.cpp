#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "gdpp/evt/Event.hpp"
#include "gdpp/evt/EventCenter.hpp"

namespace Gd { namespace Evt {

Event & EventCenter::createEvent(size_t attach_capacity, const char * typeName, ssize_t data_capacity) {
    Cpe::Utils::ErrorCollector em;
    gd_evt_t r = gd_evt_create(*this, attach_capacity, typeName, data_capacity, em);
    if (r == NULL) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }
    return *(Event *)r;
}

void EventCenter::sendEvent(Event & event) {
    int r = gd_evt_send(event, 0, 0, 1);
    if (r != 0) {
        throw ::std::runtime_error("send event fail!");
    }
}

EventCenter & EventCenter::_cast(gd_evt_mgr_t evm) {
    if (evm == NULL) {
        throw ::std::runtime_error("cast to EventCenter: input is NULL!");
    }

    return *(EventCenter*)evm;
}

}}


