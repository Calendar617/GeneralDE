#ifndef GDPP_EVT_EVENTCENTER_H
#define GDPP_EVT_EVENTCENTER_H
#include "cpepp/utils/ClassCategory.hpp"
#include "cpepp/dr/MetaLib.hpp"
#include "gdpp/app/Application.hpp"
#include "gd/evt/evt_manage.h"
#include "System.hpp"
#include "EventResponser.hpp"

namespace Gd { namespace Evt {

class EventCenter : public Cpe::Utils::SimulateObject {
public:
    operator gd_evt_mgr_t (void) const { return (gd_evt_mgr_t)this; }

    const char * name(void) const { return gd_evt_mgr_name(*this); }
    cpe_hash_string_t name_hs(void) const { return gd_evt_mgr_name_hs(*this); }

    App::Application & app(void) { return App::Application::_cast(gd_evt_mgr_app(*this)); }
    App::Application const & app(void) const { return App::Application::_cast(gd_evt_mgr_app(*this)); }

    Cpe::Dr::MetaLib const & metaLib(void) const { return *(Cpe::Dr::MetaLib*)gd_evt_mgr_metalib(*this); }

    Event & createEvent(const char * typeName, ssize_t capacity = -1);
	void sendEvent(const char * oid, Event & event);

    tl_t tl(void) const { return  gd_evt_mgr_tl(*this); }

    template<typename T>
    ProcessorID registerResponser(const char * oid, T & r, void (T::*fun)(const char * oid, Event const & e)) {
#ifdef _MSC_VER
        return this->registerResponser(oid, r, static_cast<EventProcessFun>(fun), *((EventResponser*)((void*)&r)));
#else
        return this->registerResponser(oid, static_cast<EventResponser&>(r), static_cast<EventProcessFun>(fun));
#endif
    }

    /*VC编译器处理成员函数地址时有错误，没有生成垫片函数，所以为了正确调用函数指针，必须直接把传入T类型的对象绑定在调用的对象上
      所以传入的realResponser为真实的Responser地址，而useResponser是T的this地址，用于调用函数的
     */
    ProcessorID registerResponser(
        const char * oid,
        EventResponser& realResponser, EventProcessFun fun
#ifdef _MSC_VER
        ,EventResponser& useResponser
#endif
        );

    void unregisterResponser(EventResponser & r);

    static EventCenter & instance(gd_app_context_t app, const char * name = NULL);
    static EventCenter & _cast(gd_evt_mgr_t evm);
};	

}}

#endif
