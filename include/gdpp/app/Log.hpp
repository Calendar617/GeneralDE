#ifndef GDPP_LOG_H
#define GDPP_LOG_H
#include "cpe/pal/pal_stdio.h"
#include "gd/app/app_log.h"
#include "System.hpp"

#ifdef _MSC_VER  /******************* for vc */

#define APP_THROW_EXCEPTION(__e, __format, ...)         \
    do {                                                \
        char __buf[1024];                               \
        snprintf(__buf, 1025, __format, __VA_ARGS__);   \
        APP_ERROR("%s", __buf);                         \
        throw __e( __buf );                             \
    } while(0)

#define APP_CTX_THROW_EXCEPTION(__app, __e, __format, ...)  \
    do {                                                    \
        char __buf[1024];                                   \
        snprintf(__buf, 1025, __format, __VA_ARGS__);       \
        APP_CTX_ERROR(__app, "%s", __buf);                  \
        throw __e( __buf );                                 \
    } while(0)

#else /******************* for other(gcc) */

#define APP_THROW_EXCEPTION(__e, __format, args...) \
    do {                                            \
        char __buf[1024];                           \
        snprintf(__buf, 1025, __format, ##args);    \
        APP_ERROR("%s", __buf);                     \
        throw __e( __buf );                         \
    } while(0)

#define APP_CTX_THROW_EXCEPTION(__app, __e, __format, args...)  \
    do {                                                        \
        char __buf[1024];                                       \
        snprintf(__buf, 1025, __format, ##args);                \
        APP_CTX_ERROR(__app, "%s", __buf);                      \
        throw __e( __buf );                                     \
    } while(0)

#endif

#endif


