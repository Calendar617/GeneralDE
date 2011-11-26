#ifndef GD_TL_ERRNO_H
#define GD_TL_ERRNO_H
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    GD_TL_ERROR_NONE = 0
    , GD_TL_ERROR_NO_MEMORY = -128
    , GD_TL_ERROR_NO_TIME_SOURCE
    , GD_TL_ERROR_EVENT_NO_DISPATCHER
    , GD_TL_ERROR_EVENT_UNKNOWN
};

const char * gd_tl_error_string(int e);

#ifdef __cplusplus
}
#endif

#endif
