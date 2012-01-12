#ifndef GDPP_TL_TIMER_H
#define GDPP_TL_TIMER_H
#include "System.hpp"

namespace Gd { namespace Tl {

class Timer {
public:
    virtual void onTimer(int timerId) = 0;
    virtual ~Timer() throw();
};

}}

#endif

