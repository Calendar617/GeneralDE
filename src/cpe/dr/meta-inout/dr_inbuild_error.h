#ifndef CPE_DR_BUILD_INBUILD_ERROR_H
#define CPE_DR_BUILD_INBUILD_ERROR_H
#include "cpe/dr/dr_metalib_build.h"
#include "cpe/dr/dr_error.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DR_NOTIFY_ERROR(userData, errorProcessor, e) \
    if (errorProcessor) errorProcessor(userData, NULL, 0, e, dr_error_string(e));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


