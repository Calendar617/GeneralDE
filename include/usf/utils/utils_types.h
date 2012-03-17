#ifndef USF_UTILS_TYPES_H
#define USF_UTILS_TYPES_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t usf_id_t;

#define usf_id_hash(value) ((uint32_t)(((value >> 32) & 0xFFFF) | (value & 0xFFFFFFFF)))

#ifdef __cplusplus
}
#endif

#endif


