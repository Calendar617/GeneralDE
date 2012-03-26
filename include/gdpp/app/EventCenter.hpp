#ifndef GDPP_APP_EVENTCENTER_H
#define GDPP_APP_EVENTCENTER_H
#include "gdpp/app/System.hpp"
#include "gdpp/nm/Object.hpp"
#include "Event.hpp"
#include "EventResponser.hpp"

namespace Gd { namespace App {

class EventCenter : public Gd::Nm::Object {
public:
    virtual Event & createEvent(const char * typeName, ssize_t capacity = -1) = 0;
	virtual void sendEvent(const char * oid, Event & event) = 0;
    virtual tl_t tl(void) = 0;

    template<typename T>
    void registerResponser(const char * oid, T & r, void (T::*fun)(const char * oid, Event const & e)) {
#ifdef _MSC_VER
        this->registerResponser(oid, r, *((EventResponser*)((void*)&r)), static_cast<EventProcessFun>(fun));
#else
        this->registerResponser(oid, static_cast<EventResponser&>(r), static_cast<EventResponser&>(r), static_cast<EventProcessFun>(fun));
#endif
    }

    /*VC编译器处理成员函数地址时有错误，没有生成垫片函数，所以为了正确调用函数指针，必须直接把传入T类型的对象绑定在调用的对象上
      所以传入的realResponser为真实的Responser地址，而useResponser是T的this地址，用于调用函数的
     */
	virtual void registerResponser(const char * oid, EventResponser& realResponser, EventResponser& useResponser, EventProcessFun fun) = 0;
	virtual void unregisterResponser(EventResponser & r) = 0;

    virtual ~EventCenter();

    static EventCenter & instance(Application & app, cpe_hash_string_t name = DEFAULT_NAME);

    static cpe_hash_string_t DEFAULT_NAME;
    static cpe_hash_string_t req_type_name;
};	

}}

#endif
