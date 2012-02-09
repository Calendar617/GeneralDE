#include <assert.h>
#include "cpe/utils/random.h"

uint32_t cpe_rand(uint32_t seed, uint32_t max) {
    seed = seed ^ (seed >> 11);
    seed = seed ^ ((seed << 7) & (2636928640u));
    seed = seed ^ ((seed << 15) & (4022730752u));
    seed = seed ^ ((seed >> 18));

    return seed % (max + 1);
}
