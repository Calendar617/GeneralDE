#ifndef GDPP_APP_TIMEERCENTER_H
#define GDPP_APP_TIMEERCENTER_H
#include "gdpp/nm/Object.hpp"
#include "System.hpp"
#include "TimerProcessor.hpp"

namespace Gd { namespace App {

class TimerCenter : public Gd::Nm::Object {
public:
    template<typename T>
    TimerID registerTimer(
        T & r,
        void (T::*fun)(TimerID timerId),
        tl_time_span_t delay,
        tl_time_span_t span,
        int repeatCount = -1)
    {
#ifdef _MSC_VER
        return this->registerTimer(
            r, *((TimerProcessor*)((void*)&r)), static_cast<TimerProcessFun>(fun),
            delay, span, repeatCount);
#else
        return this->registerTimer(
            static_cast<TimerProcessor&>(r), static_cast<TimerProcessor&>(r), static_cast<TimerProcessFun>(fun),
            delay, span, repeatCount);
#endif
    }

    /*VC编译器处理成员函数地址时有错误，没有生成垫片函数，所以为了正确调用函数指针，必须直接把传入T类型的对象绑定在调用的对象上
      所以传入的realResponser为真实的Responser地址，而useResponser是T的this地址，用于调用函数的
     */
	virtual TimerID registerTimer(
        TimerProcessor& realResponser, TimerProcessor& useResponser, TimerProcessFun fun,
        tl_time_span_t delay, tl_time_span_t span, int repeatCount = -1) = 0;

	virtual void unregisterTimer(TimerProcessor & r) = 0;
	virtual void unregisterTimer(TimerID timerId) = 0;

    virtual ~TimerCenter();

    static TimerCenter & instance(Application & app, cpe_hash_string_t name = DEFAULT_NAME);

    static cpe_hash_string_t DEFAULT_NAME;
};

}}

#endif


