#ifndef CPE_PAL_STDIO_H
#define CPE_PAL_STDIO_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
# define snprintf _snprintf
#elif _MSC_VER >= 1400  // VC 8.0 and later deprecate snprintf and _snprintf.
# define snprintf _snprintf_s
#elif _MSC_VER
# define snprintf _snprintf
#endif

#ifdef __cplusplus
}
#endif

#endif
