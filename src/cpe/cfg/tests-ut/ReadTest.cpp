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

int ReadTest::read(const char * input) {
    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    //CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    //CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    //return cfg_read(m_root, data, meta, &em);
    return 0;
}

const char * ReadTest::result(void) {
    CPE_DEF_ERROR_MONITOR(em, cpe_error_log_to_consol, NULL);

    mem_buffer_clear(&m_result_buffer);
    struct write_stream_buffer stream = CPE_WRITE_STREAM_BUFFER_INITIALIZER(&m_result_buffer);

    EXPECT_EQ(0, cfg_write((write_stream_t)&stream, m_root, &em));
    stream_putc((write_stream_t)&stream, 0);

    return (const char *)mem_buffer_make_exactly(&m_result_buffer);
}
