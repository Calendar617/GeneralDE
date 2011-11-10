#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "BuildFromXmlTest.hpp"

BuildFromXmlTest::BuildFromXmlTest() : m_metaLib(0) {
}

void BuildFromXmlTest::TearDown() {
    dr_free_lib(&m_metaLib);
}

static void dr_create_lib_from_xml_error_test_record(
    struct error_info * info, void * context, const char * msg)
{
    BuildFromXmlTest * t = (BuildFromXmlTest *)context;

    for(int i = 0; i < BuildFromXmlTest::MAX_ERROR_COUNT; ++i) {
        if (t->m_errors[i] == 0) {
            t->m_errors[i] = info->m_errno;
            break;
        }
    }
}

int BuildFromXmlTest::errorCount(void) {
    int i = 0;
    while(i < MAX_ERROR_COUNT && m_errors[i] != 0) {
        ++i;
    }

    return i;
}

bool BuildFromXmlTest::haveError(int error) {
    for(int i = 0; i < MAX_ERROR_COUNT; ++i) {
        if (CPE_ERR_BASE(m_errors[i]) == error) {
            return true;
        }
    }

    return false;
}

int BuildFromXmlTest::parseMeta(const char * def) {
    dr_free_lib(&m_metaLib);

    bzero(m_errors, sizeof(m_errors));

    CPE_DEF_ERROR_MONITOR(em, dr_create_lib_from_xml_error_test_record, this);

    return dr_create_lib_from_xml_ex(&m_metaLib, def, strlen(def), &em);
}

LPDRMETA
BuildFromXmlTest::get_meta(const char * name) {
    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, name);

    EXPECT_TRUE(meta != NULL) << "get meta " << name << " fail!";

    return meta;
}

LPDRMETAENTRY
BuildFromXmlTest::get_entry(const char * metaName, const char * entryName) {
    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, metaName);
    EXPECT_TRUE(meta != NULL)
        << "get meta " << metaName << " fail!";

    LPDRMETAENTRY entry = dr_get_entry_by_path(meta, entryName);
    EXPECT_TRUE(entry != NULL)
        << "get entry " << entryName << " of " << metaName << " fail!";

    return entry;
}
