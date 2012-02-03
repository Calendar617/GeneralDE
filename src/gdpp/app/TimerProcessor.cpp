#include "gdpp/app/TimerProcessorBase.hpp"

namespace Gd { namespace App {

TimerProcessor::~TimerProcessor() {
}

TimerProcessorBase::TimerProcessorBase(TimerCenter & timerCenter)
    : _timerCenter(timerCenter)
{
}

TimerProcessorBase::~TimerProcessorBase() {
    _timerCenter.unregisterTimer(*this);
}

}}

