#include <assert.h>
#include "cpe/pal/pal_stdio.h"
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

int cpe_str_buf_is_overflow(cpe_str_buf_t buf) {
    return buf->m_overflow;
}

int cpe_str_buf_cat(cpe_str_buf_t buf, const char * data) {
    size_t data_len;

    assert(buf);
    assert(data);
    assert(buf->m_buf);

    data_len = strlen(data);

    if (data_len + buf->m_size + 1 > buf->m_capacity) {
        data_len = buf->m_capacity - buf->m_size - 1;
        buf->m_overflow = 1;
    }

    if (data_len) {
        memcpy(buf->m_buf + buf->m_size, data, data_len + 1);
        buf->m_size += data_len;
    }

    return buf->m_overflow;
}

int cpe_str_buf_cpy(cpe_str_buf_t buf, const char * data) {
    size_t data_len;

    assert(buf);
    assert(data);
    assert(buf->m_buf);

    data_len = strlen(data);

    if (data_len + 1 > buf->m_capacity) {
        data_len = buf->m_capacity - 1;
        buf->m_overflow = 1;
    }
    else {
        buf->m_overflow = 0;
    }

    if (data_len) {
        memcpy(buf->m_buf, data, data_len + 1);
        buf->m_size = data_len;
    }

    return buf->m_overflow;
}

int cpe_str_buf_cat_printf(cpe_str_buf_t buf, const char * fmt, ...) {
    va_list args;
    size_t capacity;
    int r;

    assert(buf);
    assert(fmt);
    assert(buf->m_buf);

    capacity = buf->m_capacity - buf->m_size;

    va_start(args, fmt);
    r = vsnprintf(buf->m_buf + buf->m_size, capacity, fmt, args);

    if (r < 0) {
        buf->m_overflow = 1;
    }
    else {
        buf->m_size += r;
    }

    va_end(args);

    return buf->m_overflow;
}

int cpe_str_buf_printf(cpe_str_buf_t buf, const char * fmt, ...) {
    va_list args;
    int r;

    assert(buf);
    assert(fmt);
    assert(buf->m_buf);

    va_start(args, fmt);
    r = vsnprintf(buf->m_buf, buf->m_capacity, fmt, args);

    if (r < 0) {
        buf->m_overflow = 1;
    }
    else {
        buf->m_size += r;
        buf->m_overflow = 0;
    }

    va_end(args);

    return buf->m_overflow;
}
