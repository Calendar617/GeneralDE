#ifndef CPEPP_UTILS_RANDOM_H
#define CPEPP_UTILS_RANDOM_H
#include "cpe/utils/random.h"
#include "System.hpp"

namespace Cpe { namespace Utils {

class Random {
public:
    Random(int32_t seed);
    void init(int32_t seed);

    uint32_t generate(uint32_t max);

private:
    int32_t m_state_arr[624];
    int m_index;
};

}}

#endif

