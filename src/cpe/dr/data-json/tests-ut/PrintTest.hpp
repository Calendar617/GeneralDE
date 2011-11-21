#ifndef CPE_DR_DATAJSON_TEST_PRINTTEST_H
#define CPE_DR_DATAJSON_TEST_PRINTTEST_H
#include <string.h>
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/dr/dr_json.h"

class PrintTest : public ::testing::Test {
public:
    PrintTest();
    virtual void SetUp();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    struct mem_buffer m_metaLib_buffer;

    struct mem_buffer m_buffer;
    error_list_t m_errorList;

    void installMeta(const char * data);
    int print(const void * data, const char * typeName);
    const char * result(void);
};

#endif
