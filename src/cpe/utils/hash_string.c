#include <string.h>
#include "cpe/utils/hash_string.h"
#include "cpe/utils/hash.h"

#define cpe_hs_check_len_init(hs)                       \
    int32_t * __len = ((int32_t *)(hs)) + 1;            \
    if (*__len == 0) {                                  \
        *__len = strlen((char *)(__len + 1));           \
    }                                                   \

#define cpe_hs_check_init(hs)                           \
    int32_t * b;                                        \
    cpe_hs_check_len_init(hs);                          \
    b = (int32_t *)(hs);                      \
    if (*b == 0) {                                      \
        *b = cpe_hash_str((char *)(b + 2), *__len);     \
        if (*b == 0) *b = 1;                            \
    }                                                   \
    
size_t cpe_hs_len(cpe_hash_string_t hs) {
    cpe_hs_check_len_init(hs);
    return *__len;
}

size_t cpe_hs_binary_len(cpe_hash_string_t hs) {
    cpe_hs_check_len_init(hs);
    return cpe_hs_len_to_binary_len(*__len);
}

uint32_t cpe_hs_value(cpe_hash_string_t hs) {
    cpe_hs_check_init(hs);
    return *b;
}    

void cpe_hs_strcat(cpe_hash_string_t target, size_t capacity, const char * data) {
    int32_t * hash = (int32_t *)target;
    int32_t * len = hash + 1;
    char * buf = (char *)(len + 1);

    ssize_t dataLen = (ssize_t)strlen(data);
    if ((dataLen + *len) > capacity - 9) {
        dataLen = (*len) - (capacity - 9);
    }

    if (dataLen <= 0) return;

    memcpy(buf + *len, data, dataLen);
    *len += dataLen;
    buf[*len] = 0;
    *hash = 0;
}

void cpe_hs_init(cpe_hash_string_t target, size_t capacity, const char * source) {
    int32_t * hash = (int32_t *)target;
    int32_t * len = hash + 1;
    char * data = (char *)(len + 1);

    *len = strlen(source);
    if (*len > capacity - 9) {
        *len = capacity - 9;
    }

    memcpy(data, source, *len);
    data[*len] = 0;

    *hash = 0;
}

cpe_hash_string_t
cpe_hs_create(mem_allocrator_t alloc, const char * data) {
    int32_t dataLen;
    int32_t * buf;
    if (data == NULL) return NULL;

    dataLen = strlen(data);
    buf = (int32_t*)mem_alloc(alloc, 8 + dataLen + 1);
    memcpy(buf + 2, data, dataLen + 1);
    *buf = cpe_hash_str(data, dataLen);
    *(buf + 1) = dataLen;
    return (cpe_hash_string_t)buf;
}

void cpe_hs_copy(cpe_hash_string_t target, cpe_hash_string_t source) {
    memcpy(target, source, cpe_hs_binary_len(source));
}

cpe_hash_string_t 
cpe_hs_copy_create(mem_allocrator_t alloc, cpe_hash_string_t source) {
    cpe_hash_string_t r =
        (cpe_hash_string_t)mem_alloc(alloc, cpe_hs_binary_len(source));
    if (r == NULL) return NULL;
    cpe_hs_copy(r, source);
    return r;
}
