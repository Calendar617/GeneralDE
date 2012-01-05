#include <stdexcept>
#include "cpepp/utils/ErrorCollector.hpp"
#include "gdpp/evt/Event.hpp"
#include "gdpp/evt/EventCenter.hpp"

namespace Gd { namespace Evt {

Event & EventCenter::createEvent(const char * typeName) {
    Cpe::Utils::ErrorCollector em;
    gd_evt_t r = gd_evt_create(*this, typeName, em);
    if (r == NULL) {
        em.checkThrowWithMsg< ::std::runtime_error>();
    }
    return *(Event *)r;
}

}}


