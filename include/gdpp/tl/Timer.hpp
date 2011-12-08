#ifndef GD_TLPP_TIMER_H
#define GD_TLPP_TIMER_H

namespace Tlpp {

class Timer {
public:
    virtual void onTimer(int timerId) = 0;
    virtual ~Timer() throw();
};

}

#endif

