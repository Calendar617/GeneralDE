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
    void * p, const char * file, int line, int errno, const char * msg)
{
    BuildFromXmlTest * t = (BuildFromXmlTest *)p;

    for(int i = 0; i < BuildFromXmlTest::MAX_ERROR_COUNT; ++i) {
        if (t->m_errors[i] == 0) {
            t->m_errors[i] = errno;
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

    return dr_create_lib_from_xml_ex(
        &m_metaLib,
        def, strlen(def),
        this, dr_create_lib_from_xml_error_test_record);
}

