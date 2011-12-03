#ifndef CPE_DR_DATAJSON_TEST_PRINTTEST_H
#define CPE_DR_DATAJSON_TEST_PRINTTEST_H
#include <string.h>
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/utils/buffer.h"
#include "CfgTest.hpp"

class ReadTest : public CfgTest {
public:
    ReadTest();
    virtual void SetUp();
    virtual void TearDown();

    error_list_t m_errorList;

    int read(const char * input, cfg_policy_t policy = cfg_replace);
    int read(cfg_t cfg, const char * input, cfg_policy_t policy = cfg_replace);

    struct mem_buffer m_result_buffer;
    const char * result(void);
    const char * result(cfg_t cfg);
};

#endif
