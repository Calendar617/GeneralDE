#ifndef CPE_RANDOM_H
#define CPE_RANDOM_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t cpe_rand(uint32_t seed, uint32_t max);

#ifdef __cplusplus
}
#endif

#endif
