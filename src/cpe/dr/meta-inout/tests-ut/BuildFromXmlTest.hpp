#ifndef CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#define CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"

class BuildFromXmlTest : public ::testing::Test {
public:
    BuildFromXmlTest();
    virtual void SetUp();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    error_list_t m_errorList;

    int parseMeta(const char * def);

    int errorCount(void);
    bool haveError(int error);

    LPDRMETA get_meta(const char * name);
    LPDRMETAENTRY get_entry(const char * metaName, const char * entryName);
};

#endif
