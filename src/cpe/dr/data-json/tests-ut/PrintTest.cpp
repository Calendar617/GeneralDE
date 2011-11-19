#include "PrintTest.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "cpe/utils/stream_buffer.h"

PrintTest::PrintTest() : m_metaLib(0), m_errorList(0) {
}

void PrintTest::SetUp() {
    mem_buffer_init(&m_buffer, NULL);
}

void PrintTest::TearDown() {
    mem_buffer_free(&m_buffer);
    dr_lib_free(&m_metaLib);
    cpe_error_list_free(m_errorList);
}

void PrintTest::installMeta(const char * def) {
    dr_lib_free(&m_metaLib);
    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(&m_metaLib, def, strlen(def), &em))
        << "install meta error";
}

int PrintTest::print(const void * data, const char * typeName) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";

    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    struct write_stream_buffer stream = CPE_STREAM_BUFFER_INITIALIZER(&m_buffer);

    int r = dr_json_print((write_stream_t)&stream, data, meta, DR_JSON_PRINT_BEAUTIFY, &em);
    stream_putc((write_stream_t)&stream, 0);
    return r;
}

const char * PrintTest::result(void) {
    return (const char *)mem_buffer_make_continuous(&m_buffer);
}
