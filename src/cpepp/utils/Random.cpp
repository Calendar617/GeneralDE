#include "cpepp/utils/Random.hpp"

namespace Cpe { namespace Utils {

Random::Random(int32_t seed) {
    init(seed);
}

void Random::init(int32_t seed) {
    m_state_arr[0] = seed;
    m_index = 0;

    for (uint32_t i = 1; i < (sizeof(m_state_arr) / sizeof(int32_t)); ++i) {
        m_state_arr[i] = 1812433253 * (m_state_arr[i - 1] ^ (m_state_arr[i - 1] >> 30)) + i;
    }
}

uint32_t Random::generate(uint32_t max) {
    if (m_index == 0) {
		for (uint32_t i = 0; i < (sizeof(m_state_arr) / sizeof(int32_t)) ; ++i) {
			int32_t y = (m_state_arr[i] & 0x8000) + ((m_state_arr[(i + 1) % (sizeof(m_state_arr) / sizeof(int32_t))]) & 0x7fff);
			m_state_arr[i] = m_state_arr[(i + 397) % (sizeof(m_state_arr) / sizeof(int32_t))] ^ (y >> 1);

			if ( (y % 2 == 1) ) {  // y is odd
				m_state_arr[i] = m_state_arr[i] ^ 2567483615u;
			}
		}
    }

    uint32_t r = cpe_rand(m_state_arr[m_index], max);
    m_index = (m_index + 1) % (sizeof(m_state_arr) / sizeof(int32_t));
    return r;
}

}}
