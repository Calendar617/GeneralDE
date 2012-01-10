#ifndef CPE_PAL_STRING_H
#define CPE_PAL_STRING_H

#include <string.h>

#if defined _MSC_VER
#define strdup _strdup
#pragma warning(disable:4996)
#endif

#endif
