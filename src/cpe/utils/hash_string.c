#include <string.h>
#include "cpe/utils/hash_string.h"
#include "cpe/utils/hash.h"

#define cpe_hs_check_init(hs)                           \
    int32_t * b = (int32_t *)(hs);                      \
    if (*b == 0) {                                      \
        *(b + 1) = strlen((char *)(b + 2));             \
        *b = cpe_hash_str((char *)(b + 2), *(b + 1));   \
        if (*b == 0) *b = 1;                            \
    }                                                   \
    
size_t cpe_hs_len(cpe_hash_string_t hs) {
    cpe_hs_check_init(hs);
    return *(b + 1);
}

size_t cpe_hs_binary_len(cpe_hash_string_t hs) {
    cpe_hs_check_init(hs);
    return *(b + 1) + 8;
}

uint32_t cpe_hs_value(cpe_hash_string_t hs) {
    cpe_hs_check_init(hs);
    return *b;
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
