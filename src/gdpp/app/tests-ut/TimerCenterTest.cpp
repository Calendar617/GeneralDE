#include "cpe/pal/pal_dlfcn.h"
#include "TimerCenterTest.hpp"

void TimerCenterTest::SetUp() {
    Base::SetUp();
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    t_app_install_module("AppTimerCenter", NULL, "");

    t_app_set_timer_source_last_event();
}

void TimerCenterTest::TearDown() {
    t_app_uninstall_module("AppTimerCenter");
    Base::TearDown();
}

Gd::App::TimerCenterExt &
TimerCenterTest::timerCenter(void) {
    return dynamic_cast<Gd::App::TimerCenterExt &>(
        Gd::App::TimerCenter::instance(t_app_ex()));
}
