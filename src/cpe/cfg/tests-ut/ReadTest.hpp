#ifndef CPE_DR_DATAJSON_TEST_PRINTTEST_H
#define CPE_DR_DATAJSON_TEST_PRINTTEST_H
#include <string.h>
#include "cpe/utils/tests-env/with_em.hpp"
#include "CfgTest.hpp"

class ReadTest
    : public testenv::fixture<LOKI_TYPELIST_1(utils::testenv::with_em), CfgTest>
{
public:
    ReadTest();
    virtual void SetUp();
    virtual void TearDown();

    int read(const char * input, cfg_policy_t policy = cfg_replace);
    int read(cfg_t cfg, const char * input, cfg_policy_t policy = cfg_replace);

    struct mem_buffer m_result_buffer;
    const char * result(void);
    const char * result(cfg_t cfg);
};

#endif
