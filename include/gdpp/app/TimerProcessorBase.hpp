#ifndef GDPP_APP_TIMERRESPONSER_BASE_H
#define GDPP_APP_TIMERRESPONSER_BASE_H
#include "TimerResponser.hpp"
#include "TimerCenter.hpp"

namespace Gd { namespace App {

class TimerResponserBase : public TimerResponser {
public:
	TimerResponserBase(TimerCenter & eventCenter);
	~TimerResponserBase();

private:	
	TimerCenter & _eventCenter;
};

}}

#endif
