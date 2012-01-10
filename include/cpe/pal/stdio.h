#ifndef CPE_PAL_STDIO_H
#define CPE_PAL_STDIO_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined _MSC_VER
#if _MSC_VER >= 1400  // VC 8.0 and later deprecate snprintf and _snprintf.
# define snprintf(__buf, __size, __fmt, ...) _snprintf_s(__buf, __size, 128, __fmt, __VA_ARGS__)
#else
# define snprintf _snprintf
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
