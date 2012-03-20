#ifndef GDPP_APP_TIMERRESPONSER_BASE_H
#define GDPP_APP_TIMERRESPONSER_BASE_H
#include "TimerProcessor.hpp"
#include "TimerCenter.hpp"

namespace Gd { namespace App {

class TimerProcessorBase : public TimerProcessor {
public:
	TimerProcessorBase(TimerCenter & eventCenter);
	~TimerProcessorBase();

private:	
	TimerCenter & _timerCenter;
};

}}

#endif
