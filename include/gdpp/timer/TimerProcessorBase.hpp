#ifndef GDPP_TIMER_PROCESSOR_BASE_H
#define GDPP_TIMER_PROCESSOR_BASE_H
#include "TimerProcessor.hpp"
#include "TimerCenter.hpp"

namespace Gd { namespace Timer {

class TimerProcessorBase : public TimerProcessor {
public:
	TimerProcessorBase(TimerCenter & eventCenter);
	~TimerProcessorBase();

private:	
	TimerCenter & _eventCenter;
};

}}

#endif
