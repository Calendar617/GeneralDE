#ifndef GD_UTILS_TYPES_H
#define GD_UTILS_TYPES_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t gd_id_t;
typedef struct gd_id_generator * gd_id_generator_t;

#define gd_id_hash(value) ((uint32_t)(((value >> 32) & 0xFFFF) | (value & 0xFFFFFFFF)))

#ifdef __cplusplus
}
#endif

#endif


