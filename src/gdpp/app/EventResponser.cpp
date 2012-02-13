#include "gdpp/app/EventResponserBase.hpp"

namespace Gd { namespace App {

EventResponser::~EventResponser() {
}

EventResponserBase::EventResponserBase(EventCenter & eventCenter)
    : _eventCenter(eventCenter)
{
}

EventResponserBase::~EventResponserBase() {
    _eventCenter.unregisterResponser(*this);
}

}}

