#include "cpe/utils/stream_buffer.h"
#include "RangeAllocratorTest.hpp"

void RangeAllocratorTest::SetUp(void) {
    Base::SetUp();

    EXPECT_EQ(
        0,
        cpe_range_allocrator_init(&m_ra, t_allocrator()));

    mem_buffer_init(&m_dump_buffer, t_tmp_allocrator());
}

void RangeAllocratorTest::TearDown(void) {
    mem_buffer_clear(&m_dump_buffer);
    cpe_range_allocrator_fini(&m_ra);

    Base::TearDown();
}

const char * RangeAllocratorTest::dump(void) {
    mem_buffer_clear_data(&m_dump_buffer);

    struct write_stream_buffer ws = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&m_dump_buffer);

    cpe_range_allocrator_dump((write_stream_t)&ws, &m_ra);

    mem_buffer_append_char(&m_dump_buffer, 0);

    return (char *)mem_buffer_make_continuous(&m_dump_buffer, 0);
}


