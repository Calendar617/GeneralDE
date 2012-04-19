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

typedef struct cpe_str_buf {
    size_t m_capacity;
    size_t m_size;
    char * m_buf;
    int m_overflow;
} * cpe_str_buf_t;

int cpe_str_buf_is_overflow(cpe_str_buf_t buf);
int cpe_str_buf_cat(cpe_str_buf_t buf, const char * data);
int cpe_str_buf_cat_printf(cpe_str_buf_t buf, const char * format, ...);
int cpe_str_buf_cpy(cpe_str_buf_t buf, const char * data);
int cpe_str_buf_printf(cpe_str_buf_t buf, const char * format, ...);

#define CPE_STR_BUF_INIT(__b, __size) { __size, 0, __b, 0 }

#ifdef __cplusplus
}
#endif

#endif
