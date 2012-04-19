#include "cpe/pal/pal_string.h"
#include "cpe/utils/string_utils.h"

char *
cpe_str_dup_range(char * buf, size_t capacity, const char * begin, const char * end) {
    int size;

    if (buf == NULL) return NULL;

    size = end - begin;
    if (size < 0 || ((size_t)size) + 1 > capacity) return NULL;

    memcpy(buf, begin, size);
    buf[size] = 0;

    return buf;
}

char *
cpe_str_dup_len(char * buf, size_t capacity, const char * begin, size_t size) {
    if (buf == NULL) return NULL;

    if (size + 1 > capacity) return NULL;

    memcpy(buf, begin, size);
    buf[size] = 0;

    return buf;
}
