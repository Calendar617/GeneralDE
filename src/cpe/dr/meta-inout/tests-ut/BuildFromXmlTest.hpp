#ifndef CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#define CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_metalib_xml.h"
#include "../../dr_internal_types.h"

class BuildFromXmlTest : public ::testing::Test {
public:
    BuildFromXmlTest();
    virtual void SetUp();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    struct mem_buffer m_buffer;
    error_list_t m_errorList;

    int parseMeta(const char * def);

    int errorCount(void);
    bool haveError(int error);

    LPDRMETA meta(const char * name);
    LPDRMETAENTRY entry(const char * metaName, const char * entryName);

    int32_t address_to_pos(void * p);
};

#endif
