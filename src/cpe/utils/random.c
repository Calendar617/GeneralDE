#include <assert.h>
#include "cpe/utils/random.h"

uint32_t cpe_rand(int32_t seed, uint32_t max) {
    seed = seed ^ (seed >> 11);
    seed = seed ^ ((seed << 7) & (2636928640u));
    seed = seed ^ ((seed << 15) & (4022730752u));
    seed = seed ^ ((seed >> 18));

    return seed % (max + 1);
}

void cpe_rand_ctx_init(struct cpe_rand_ctx * ctx, int32_t seed) {
    uint32_t i;

    ctx->m_state_arr[0] = seed;
    ctx->m_index = 0;

    for (i = 1; i < (sizeof(ctx->m_state_arr) / sizeof(int32_t)); ++i) {
        ctx->m_state_arr[i] = 1812433253 * (ctx->m_state_arr[i - 1] ^ (ctx->m_state_arr[i - 1] >> 30)) + i;
    }
}

uint32_t cpe_rand_ctx_generate(struct cpe_rand_ctx * ctx, uint32_t max) {
    uint32_t r;

    if (ctx->m_index == 0) {
        uint32_t i;
		for (i = 0; i < (sizeof(ctx->m_state_arr) / sizeof(int32_t)) ; ++i) {
			int32_t y = (ctx->m_state_arr[i] & 0x8000) + ((ctx->m_state_arr[(i + 1) % (sizeof(ctx->m_state_arr) / sizeof(int32_t))]) & 0x7fff);
			ctx->m_state_arr[i] = ctx->m_state_arr[(i + 397) % (sizeof(ctx->m_state_arr) / sizeof(int32_t))] ^ (y >> 1);

			if ( (y % 2 == 1) ) {  // y is odd
				ctx->m_state_arr[i] = ctx->m_state_arr[i] ^ 2567483615u;
			}
		}
    }

    r = cpe_rand(ctx->m_state_arr[ctx->m_index], max);
    ctx->m_index = (ctx->m_index + 1) % (sizeof(ctx->m_state_arr) / sizeof(int32_t));
    return r;
}

struct cpe_rand_ctx g_cpe_rand_ctx_dft;
