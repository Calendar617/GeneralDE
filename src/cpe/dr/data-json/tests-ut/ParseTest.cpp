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
    mem_buffer_clear(&m_buffer);
    m_metaLib = NULL;
    cpe_error_list_free(m_errorList);
}

void ParseTest::installMeta(const char * def) {
    m_metaLib = NULL;
    mem_buffer_clear(&m_buffer);

    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(&m_buffer, def, strlen(def), &em))
        << "install meta error";

    m_metaLib = (LPDRMETALIB)mem_buffer_make_exactly(&m_buffer);
}

int ParseTest::read(const char * data, const char * typeName) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";

    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    //CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    return dr_json_read(&m_buffer, data, meta, &em);
}

void * ParseTest::result(int startPos) {
    return ((char *)mem_buffer_make_exactly(&m_buffer)) + startPos;
}
