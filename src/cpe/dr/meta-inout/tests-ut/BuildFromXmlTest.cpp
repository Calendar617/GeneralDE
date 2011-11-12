#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "BuildFromXmlTest.hpp"

BuildFromXmlTest::BuildFromXmlTest() : m_metaLib(0), m_errorList(NULL) {
}

void BuildFromXmlTest::SetUp() {
    m_errorList = cpe_error_list_create(NULL);
    ASSERT_TRUE(m_errorList);
}

void BuildFromXmlTest::TearDown() {
    cpe_error_list_free(m_errorList);
    m_errorList = NULL;
    dr_lib_free(&m_metaLib);
}

int BuildFromXmlTest::errorCount(void) {
    return cpe_error_list_error_count(m_errorList);
}

bool BuildFromXmlTest::haveError(int e) {
    return cpe_error_list_have_errno(m_errorList, e);
}

int BuildFromXmlTest::parseMeta(const char * def) {
    dr_lib_free(&m_metaLib);

    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    //CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    return dr_create_lib_from_xml_ex(&m_metaLib, def, strlen(def), &em);
}

LPDRMETA
BuildFromXmlTest::get_meta(const char * name) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, name);

    EXPECT_TRUE(meta != NULL) << "get meta " << name << " fail!";

    return meta;
}

LPDRMETAENTRY
BuildFromXmlTest::get_entry(const char * metaName, const char * entryName) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, metaName);
    EXPECT_TRUE(meta != NULL)
        << "get meta " << metaName << " fail!";

    LPDRMETAENTRY entry = dr_meta_find_entry_by_path(meta, entryName);
    EXPECT_TRUE(entry != NULL)
        << "get entry " << entryName << " of " << metaName << " fail!";

    return entry;
}
