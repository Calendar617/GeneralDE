#include "cpe/utils/stream_mem.h"
#include "cpe/utils/stream_buffer.h"
#include "ReadTest.hpp"

ReadTest::ReadTest() : m_errorList(0) {
}

void ReadTest::SetUp() {
    CfgTest::SetUp();
    mem_buffer_init(&m_result_buffer, NULL);
}

void ReadTest::TearDown() {
    mem_buffer_clear(&m_result_buffer);
    cpe_error_list_free(m_errorList);

    CfgTest::TearDown();
}

int ReadTest::read(const char * input, cfg_read_policy_t policy) {
    return read(m_root, input, policy);
}

int ReadTest::read(cfg_t cfg, const char * input, cfg_read_policy_t policy) {
    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    struct read_stream_mem stream = CPE_READ_STREAM_MEM_INITIALIZER(input, strlen(input));
    return cfg_read(cfg, (read_stream_t)&stream, policy, &em);
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
