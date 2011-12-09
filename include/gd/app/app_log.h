#ifndef GD_APP_LOG_H
#define GD_APP_LOG_H
#include <stdio.h>
#include "app_basic.h"

#define _GD_APP_DO_ERROR_NOTIFY(level, no, format, args...)             \
    if (g_app_context) {                                                \
    _CPE_DO_ERROR_NOTIFY(                                               \
        gd_app_em(g_app_context), level, no, format, ##args);           \
    } else printf(format, ##args)                                       \

#define APP_INFO(format, args...)                           \
    _GD_APP_DO_ERROR_NOTIFY(CPE_EL_INFO, 0, format, ##args)

#define APP_WARNING(format, args...)                            \
    _GD_APP_DO_ERROR_NOTIFY(CPE_EL_WARNING, -1, format, ##args)

#define APP_ERROR(format, args...)                              \
    _GD_APP_DO_ERROR_NOTIFY(CPE_EL_ERROR, -1, format, ##args)

#define APP_WARNING_EX(monitor, en, format, args...)                    \
    _GD_APP_DO_ERROR_NOTIFY(monitor, CPE_EL_WARNING, en, format, ##args)

#define APP_ERROR_EX(monitor, en, format, args...)                      \
    _GD_APP_DO_ERROR_NOTIFY(monitor, CPE_EL_ERROR, en, format, ##args)

#endif

