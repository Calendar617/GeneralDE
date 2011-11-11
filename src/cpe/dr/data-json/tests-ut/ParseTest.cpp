#include "ParseTest.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"

ParseTest::ParseTest() : m_metaLib(0), m_errorList(0) {
}

void ParseTest::SetUp() {
    mem_buffer_init(&m_buffer, NULL);
}

void ParseTest::TearDown() {
    mem_buffer_free(&m_buffer);
    dr_free_lib(&m_metaLib);
    cpe_error_list_free(m_errorList);
}

void ParseTest::installMeta(const char * def) {
    static const int MAX_ERROR_COUNT = 10;

    dr_free_lib(&m_metaLib);
    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(&m_metaLib, def, strlen(def), &em))
        << "install meta error";
}

int ParseTest::read(const char * data, const char * typeName) {
    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";
    return dr_json_read(&m_buffer, data, meta);
}

void * ParseTest::result(void) {
    return mem_buffer_make_continuous(&m_buffer);
}
