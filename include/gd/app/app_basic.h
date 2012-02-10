#ifndef GD_APP_BASIC_H
#define GD_APP_BASIC_H
#include "cpe/utils/error.h"
#include "app_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern gd_app_context_t g_app_context;
error_monitor_t gd_app_em(gd_app_context_t context);

#ifdef __cplusplus
}
#endif

#endif

