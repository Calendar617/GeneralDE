#ifndef GDPP_LOG_H
#define GDPP_LOG_H
#include "cpe/pal/stdio.h"
#include "gd/app/app_log.h"
#include "System.hpp"

#define APP_THROW_EXCEPTION(__e, __format, args...) \
    do {                                            \
        char __buf[1024];                           \
        snprintf(__buf, 1025, __format, ##args);    \
        APP_ERROR("%s", __buf);                     \
        throw __e( __buf );                         \
    } while(0)

#endif


