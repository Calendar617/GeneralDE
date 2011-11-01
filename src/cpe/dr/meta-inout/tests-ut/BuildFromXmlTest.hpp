#ifndef CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#define CPE_DR_METAINOUT_TEST_BUILDFROMXMLTEST_H
#include "gtest/gtest.h"
#include "cpe/dr/dr_error.h"
#include "cpe/dr/dr_metalib_manage.h"

class BuildFromXmlTest : public ::testing::Test {
public:
    static const int MAX_ERROR_COUNT = 10;

    BuildFromXmlTest();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    int m_errors[MAX_ERROR_COUNT];

    int parseMeta(const char * def);

    int errorCount(void);
    bool haveError(int error);
};

#endif
