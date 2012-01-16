#include "cpe/pal/pal_dlfcn.h"
#include "TimerCenterTest.hpp"

void TimerCenterTest::SetUp() {
    Base::SetUp();
    gd_set_default_library(dlopen(NULL, RTLD_NOW));
    t_app_install_module("AppTimerCenter", NULL, "");
}

void TimerCenterTest::TearDown() {
    Base::TearDown();
}

Gd::App::TimerCenter &
TimerCenterTest::timerCenter(void) {
    return Gd::App::TimerCenter::instance(t_app_ex());
}
