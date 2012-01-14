#ifndef CPE_DR_DATAJSON_TEST_PARSETEST_H
#define CPE_DR_DATAJSON_TEST_PARSETEST_H
#include <string.h>
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/dr/dr_json.h"

class ParseTest : public ::testing::Test {
public:
    ParseTest();
    virtual void SetUp();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    struct mem_buffer m_metaLib_buffer;

    struct mem_buffer m_buffer;
    error_list_t m_errorList;

    void installMeta(const char * data);
    int read(const char * data, const char * typeName);
    void * result(int startPos = 0);
};

#define ASSERT_JSON_READ_RESULT(__expect)  do {                 \
    ASSERT_TRUE(result()) << "no data output!";                 \
    ASSERT_EQ(sizeof(__expect), mem_buffer_size(&m_buffer))     \
        << "output size error!" ;                               \
    ASSERT_EQ(0, memcmp(&__expect, result(), sizeof(__expect))) \
        << "data error!";                                       \
    } while(0)

#endif
