#ifndef CPE_PAL_TYPES_H
#define CPE_PAL_TYPES_H
#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>

#ifndef WIN32
#include <wchar.h>
#include <limits.h>

#if _MSC_VER >= 1400
#  include <crtdefs.h>
#endif

#endif

#ifdef WIN32

typedef int socklen_t;
typedef int ssize_t;
typedef long suseconds_t;

typedef __int64 off64_t;
typedef long off_t;

#ifndef __int8_t_defined
# define __int8_t_defined
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;

#if _MSC_VER >= 1300
typedef long long int64_t;
#else /* _MSC_VER */
typedef __int64 int64_t;
#endif /* _MSC_VER */
#endif

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

#ifndef __uint32_t_defined
typedef unsigned int uint32_t;
# define __uint32_t_defined
#endif

#if _MSC_VER >= 1300
typedef unsigned long long uint64_t;
#else /* _MSC_VER */
typedef unsigned __int64 uint64_t;
#endif /* _MSC_VER */

typedef int mode_t;

typedef int key_t;

typedef int pid_t;
typedef int uid_t;
typedef int gid_t;

#endif /* WIN32 */

#if (__WORDSIZE == 64)
typedef int64_t ptr_int_t;
#else
typedef int32_t ptr_int_t;
#endif

#define CPE_PAL_ALIGN_8(__value) ((((__value) >> 3) + 1) << 3)
#define CPE_PAL_ALIGN_4(__value) ((((__value) >> 2) + 1) << 2)
#define CPE_PAL_ALIGN_2(__value) ((((__value) >> 1) + 1) << 1)

#endif /* TTYPES_H */

