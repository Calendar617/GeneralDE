#ifndef CPE_CFG_TEST_TLTEST_H
#define CPE_CFG_TEST_TLTEST_H
#include "cpe/utils/stream_mem.h"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/cfg/cfg.h"

typedef LOKI_TYPELIST_1(utils::testenv::with_em) CfgTestBase;
 
class CfgTest : public testenv::fixture<CfgTestBase> {
public:
    CfgTest();

    virtual void SetUp();
    virtual void TearDown();

    cfg_t m_root;
    struct mem_buffer m_result_buffer;
    const char * result(void);
    const char * result(cfg_t cfg);
};

#endif
