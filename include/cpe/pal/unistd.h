#ifndef CPE_PAL_UNISTD_H
#define CPE_PAL_UNISTD_H

#if defined _MSC_VER
#include <io.h>
#include <process.h>
#include <direct.h>
#define mkdir(__f, __m) _mkdir(__f)
#else
#include <unistd.h>
#endif

#endif
