#ifndef CPE_PAL_DIRENT_H
#define CPE_PAL_DIRENT_H

#if defined _MSC_VER
#include "msvc_dirent.h"
# define readdir_r(__dir, __buf, __dp) ((*(__dp) = readdir(__dir)) == NULL ? 0 : -1)
#else
#include <dirent.h>
#endif

#if defined _MSC_VER
# define MAXNAMLEN (256)
#endif

#endif