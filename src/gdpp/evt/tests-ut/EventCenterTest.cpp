#include "cpe/pal/pal_dlfcn.h"
#include "EventCenterTest.hpp"

void EventCenterTest::SetUp() {
    Base::SetUp();
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    t_app_install_module(
        "AppEventCenter",
        NULL
        ,
        "load-from-memory:\n"
        "    <metalib tagsetversion='1' name='event'  version='1'>\n"
        "    <struct name='event1' version='1'>\n"
        "        <entry name='a' type='int32'/>\n"
        "    </struct>\n"
        "    </metalib>\n"
        );

    t_app_set_timer_source_last_event();
}

void EventCenterTest::TearDown() {
    t_app_uninstall_module("AppEventCenter");
    Base::TearDown();
}

Gd::Evt::EventCenter &
EventCenterTest::eventCenter(void) {
    return Gd::Evt::EventCenter::instance(t_app_ex());
}
