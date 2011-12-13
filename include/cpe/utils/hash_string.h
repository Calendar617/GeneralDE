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

#define cpe_hs_data(hs) (((const char *)(hs)) + 8)
 
#define CPE_HS_LEN_TO_BINARY_LEN(__len) ((__len) + 8)
#define CPE_HS_BUF_MAKE(__str) {"\0\0\0\0\0\0\0\0" __str}


#ifdef __cplusplus
}
#endif

#endif
