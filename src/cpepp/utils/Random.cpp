#include "cpepp/utils/Random.hpp"

namespace Cpe { namespace Utils {

Random::~Random() {
}

RandomAdapter::RandomAdapter(cpe_rand_ctx & ctx)
    : m_ctx(ctx)
{
}

uint32_t RandomAdapter::generate(uint32_t max) {
    return cpe_rand_ctx_generate(&m_ctx, max);
}

static RandomAdapter s_ins(*cpe_rand_ctx_dft());

Random &
Random::dft(void) {
    return s_ins;
}

}}
