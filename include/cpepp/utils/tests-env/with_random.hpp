#ifndef CPEPP_UTILS_TESTENV_MOCK_RANDOM_H
#define CPEPP_UTILS_TESTENV_MOCK_RANDOM_H
#include "gmock/gmock.h"
#include "cpe/utils/tests-env/test-env.hpp"
#include "../Random.hpp"

namespace Cpe { namespace Utils { namespace testenv {

class with_random : public ::testenv::env<> {
public:
    class RandomMock : public Random {
    public:
        MOCK_METHOD1(generate, uint32_t(uint32_t));
    };

    RandomMock & t_random_mock(void);
    void t_random_expect_once(uint32_t arg, uint32_t r);
    void t_random_expect_once(uint32_t r);

private:
    RandomMock m_random_mock;
};

}}}

#endif