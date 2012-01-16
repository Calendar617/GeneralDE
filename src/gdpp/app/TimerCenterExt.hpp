#ifndef GDPP_APP_TIMEERCENTEREXT_H
#define GDPP_APP_TIMEERCENTEREXT_H
#include "gdpp/app/TimerCenter.hpp"

namespace Gd { namespace App {

class TimerCenterExt : public TimerCenter {
public:
    virtual bool haveTimer(TimerID) const = 0;

    virtual ~TimerCenterExt();
};

}}

#endif
