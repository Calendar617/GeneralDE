#ifndef CPE_UTILS_HASHSTRING_H
#define CPE_UTILS_HASHSTRING_H
#include "cpe/pal/types.h"
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cpe_hash_string * cpe_hash_string_t;

typedef char cpe_hash_string_buf[];

size_t cpe_hs_len(cpe_hash_string_t hs);
size_t cpe_hs_binary_len(cpe_hash_string_t hs);
uint32_t cpe_hs_value(cpe_hash_string_t hs);

cpe_hash_string_t cpe_hs_create(mem_allocrator_t alloc, const char * data);
void cpe_hs_copy(cpe_hash_string_t target, cpe_hash_string_t source);
cpe_hash_string_t cpe_hs_copy_create(mem_allocrator_t alloc, cpe_hash_string_t source);

#define cpe_hs_data(hs) (((const char *)(hs)) + 8)

#define cpe_hs_cmp(__l, __r)                                    \
    ( cpe_hs_value(__l) == cpe_hs_value(__r)                    \
      ? strcmp(cpe_hs_data(__l), cpe_hs_data(__r))              \
      : (((int)cpe_hs_value(__l)) - ((int)cpe_hs_value(__r))))

#define CPE_HS_LEN_TO_BINARY_LEN(__len) ((__len) + 8)
#define CPE_HS_BUF_MAKE(__str) {"\0\0\0\0\0\0\0\0" __str}


#ifdef __cplusplus
}
#endif

#endif
