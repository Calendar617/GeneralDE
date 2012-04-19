#ifndef CPE_UTILS_STRING_H
#define CPE_UTILS_STRING_H
#include "stream.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

char *
cpe_str_dup_range(char * buf, size_t capacity, const char * begin, const char * end);

char *
cpe_str_dup_len(char * buf, size_t capacity, const char * begin, size_t size);

#ifdef __cplusplus
}
#endif

#endif
