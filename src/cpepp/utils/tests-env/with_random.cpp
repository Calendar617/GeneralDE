#include "cpepp/utils/tests-env/with_random.hpp"

namespace Cpe { namespace Utils { namespace testenv {

with_random::RandomMock &
with_random::t_random_mock(void) {
    return m_random_mock;
}

void with_random::t_random_expect_once(uint32_t arg, uint32_t r) {
    EXPECT_CALL(m_random_mock, generate(::testing::Eq(arg)))
        .WillOnce(::testing::Return(r));
}

void with_random::t_random_expect_once(uint32_t r) {
    EXPECT_CALL(m_random_mock, generate(::testing::_))
        .WillOnce(::testing::Return(r));
}

}}}
