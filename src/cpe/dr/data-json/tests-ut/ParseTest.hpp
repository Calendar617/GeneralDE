#ifndef CPE_DR_DATAJSON_TEST_PARSETEST_H
#define CPE_DR_DATAJSON_TEST_PARSETEST_H
#include "gtest/gtest.h"
#include "cpe/dr/dr_json.h"

class ParseTest : public ::testing::Test {
public:
    ParseTest();
    virtual void SetUp();
    virtual void TearDown();

    LPDRMETALIB m_metaLib;
    struct mem_buffer m_buffer;

    void installMeta(const char * data);
    int read(char * data, int version);
};

#endif
