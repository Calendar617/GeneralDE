#ifndef GDPP_EVT_RESPONSER_BASE_H
#define GDPP_EVT_RESPONSER_BASE_H
#include "EventResponser.hpp"
#include "EventCenter.hpp"

namespace Gd { namespace Evt {

class EventResponserBase : public EventResponser {
public:
	EventResponserBase(EventCenter & eventCenter);
	~EventResponserBase();

private:	
	EventCenter & _eventCenter;
};

}}

#endif
