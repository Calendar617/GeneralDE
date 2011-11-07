#include "ParseTest.hpp"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"

ParseTest::ParseTest() : m_metaLib(0) {
}

void ParseTest::SetUp() {
    mem_buffer_init(&m_buffer, NULL);
}

void ParseTest::TearDown() {
    mem_buffer_free(&m_buffer);
    dr_free_lib(&m_metaLib);
}

void ParseTest::installMeta(const char * def) {
    static const int MAX_ERROR_COUNT = 10;

    dr_free_lib(&m_metaLib);

    EXPECT_EQ(
        0,
        dr_create_lib_from_xml_ex(&m_metaLib, def, strlen(def), NULL, NULL))
        << "install meta error";
}

int ParseTest::read(const char * data, const char * typeName, int version) {
    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, typeName);
    EXPECT_TRUE(meta) << "get meta " << typeName << " error!";
    return dr_json_read(&m_buffer, data, meta, version);
}

void * ParseTest::result(void) {
    return mem_buffer_make_continuous(&m_buffer);
}
