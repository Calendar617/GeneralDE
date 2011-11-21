#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_init.h"
#include "BuildFromXmlTest.hpp"

BuildFromXmlTest::BuildFromXmlTest() : m_metaLib(0), m_errorList(NULL) {
}

void BuildFromXmlTest::SetUp() {
    m_errorList = cpe_error_list_create(NULL);
    ASSERT_TRUE(m_errorList);

    mem_buffer_init(&m_buffer, NULL);
}

void BuildFromXmlTest::TearDown() {
    mem_buffer_clear(&m_buffer);

    cpe_error_list_free(m_errorList);
    m_errorList = NULL;

    m_metaLib = NULL;
}

int BuildFromXmlTest::errorCount(void) {
    return cpe_error_list_error_count(m_errorList);
}

bool BuildFromXmlTest::haveError(int e) {
    return cpe_error_list_have_errno(m_errorList, e);
}

int BuildFromXmlTest::parseMeta(const char * def) {
    mem_buffer_clear(&m_buffer);
    m_metaLib = NULL;

    cpe_error_list_free(m_errorList);
    m_errorList = cpe_error_list_create(NULL);

    CPE_DEF_ERROR_MONITOR(em, cpe_error_list_collect, m_errorList);
    CPE_DEF_ERROR_MONITOR_ADD(printer, &em, cpe_error_log_to_consol, NULL);

    int r = dr_create_lib_from_xml_ex(&m_buffer, def, strlen(def), &em);
    m_metaLib = (LPDRMETALIB)mem_buffer_make_continuous(&m_buffer, 0);

    return r;
}

LPDRMETA
BuildFromXmlTest::meta(const char * name) {
    LPDRMETA m = dr_lib_find_meta_by_name(m_metaLib, name);

    EXPECT_TRUE(m != NULL) << "get meta " << name << " fail!";

    return m;
}

LPDRMETAENTRY
BuildFromXmlTest::entry(const char * metaName, const char * entryName) {
    LPDRMETA meta = dr_lib_find_meta_by_name(m_metaLib, metaName);
    EXPECT_TRUE(meta != NULL)
        << "get meta " << metaName << " fail!";

    LPDRMETAENTRY entry = dr_meta_find_entry_by_path(meta, entryName);
    EXPECT_TRUE(entry != NULL)
        << "get entry " << entryName << " of " << metaName << " fail!";

    return entry;
}

int32_t BuildFromXmlTest::address_to_pos(void * p) {
    return (int32_t)((char *)p - (char *)(m_metaLib + 1));
}

