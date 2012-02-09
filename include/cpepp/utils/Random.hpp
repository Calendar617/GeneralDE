#ifndef CPEPP_UTILS_RANDOM_H
#define CPEPP_UTILS_RANDOM_H
#include "cpe/utils/random.h"
#include "System.hpp"

namespace Cpe { namespace Utils {

class Random {
public:
    virtual uint32_t generate(uint32_t max) = 0;
    virtual ~Random();

    static Random & dft(void);
};

class RandomAdapter : public Random {
public:
    RandomAdapter(cpe_rand_ctx & ctx);
    virtual uint32_t generate(uint32_t max);

private:
    cpe_rand_ctx & m_ctx;
};

}}

#endif

