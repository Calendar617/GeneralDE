#include "cpe/utils/stream_mem.h"
#include "cpe/utils/stream_buffer.h"
#include "ReadTest.hpp"

ReadTest::ReadTest() {
}

void ReadTest::SetUp() {
    Base::SetUp();
    mem_buffer_init(&m_result_buffer, NULL);
}

void ReadTest::TearDown() {
    mem_buffer_clear(&m_result_buffer);
    Base::TearDown();
}

int ReadTest::read(const char * input, cfg_policy_t policy) {
    return read(m_root, input, policy);
}

int ReadTest::read(cfg_t cfg, const char * input, cfg_policy_t policy) {
    struct read_stream_mem stream = CPE_READ_STREAM_MEM_INITIALIZER(input, strlen(input));
    return cfg_read(cfg, (read_stream_t)&stream, policy, t_em());
}

const char * ReadTest::result(void) {
    return result(m_root);
}

const char * ReadTest::result(cfg_t cfg) {
    CPE_DEF_ERROR_MONITOR(em, cpe_error_log_to_consol, NULL);

    mem_buffer_clear(&m_result_buffer);
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&m_result_buffer);

    EXPECT_EQ(0, cfg_write((write_stream_t)&stream, cfg, &em));
    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_exactly(&m_result_buffer);
}
