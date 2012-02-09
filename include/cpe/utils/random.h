#ifndef CPE_RANDOM_H
#define CPE_RANDOM_H
#include "cpe/pal/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cpe_rand_ctx {
    int32_t m_state_arr[624];
    int m_index;
};

uint32_t cpe_rand(int32_t seed, uint32_t max);

void cpe_rand_ctx_init(struct cpe_rand_ctx * ctx, int32_t seed);
uint32_t cpe_rand_ctx_generate(struct cpe_rand_ctx * ctx, uint32_t max);

struct cpe_rand_ctx * cpe_rand_ctx_dft(void);

#define cpe_rand_dft(max) cpe_rand_ctx_generate(cpe_rand_ctx_dft());

#ifdef __cplusplus
}
#endif

#endif
