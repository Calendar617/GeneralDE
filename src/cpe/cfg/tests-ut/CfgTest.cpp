#include "cpe/utils/stream_buffer.h"
#include "CfgTest.hpp"

CfgTest::CfgTest()
    : m_root(NULL)
{
}

void CfgTest::SetUp() {
    Base::SetUp();

    m_root = cfg_create(t_allocrator());
    ASSERT_TRUE(m_root) << "manage create fail!";

    mem_buffer_init(&m_result_buffer, NULL);
}

void CfgTest::TearDown() {
    cfg_free(m_root);
    m_root = NULL;

    mem_buffer_clear(&m_result_buffer);

    Base::TearDown();
}

const char * CfgTest::result(void) {
    return result(m_root);
}

const char * CfgTest::result(cfg_t cfg) {
    CPE_DEF_ERROR_MONITOR(em, cpe_error_log_to_consol, NULL);

    mem_buffer_clear(&m_result_buffer);
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&m_result_buffer);

    EXPECT_EQ(0, cfg_write((write_stream_t)&stream, cfg, &em));
    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_exactly(&m_result_buffer);
}
