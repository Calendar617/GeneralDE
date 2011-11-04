#ifndef CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#define CPE_DR_TEST_WITH_INPUTMETALIBTEST_H
#include "gtest/gtest.h"
#include "cpe/mem/buffer.h"

class BufferTest : public ::testing::Test {
public:
    virtual void SetUp();
    virtual void TearDown();

    struct mem_buffer m_buffer;

    int append_string(const char * data);
    int append_zero();

    struct mem_buffer_trunk *
    append_trunk(const char * data);

    char * as_string(void);
};

#endif
