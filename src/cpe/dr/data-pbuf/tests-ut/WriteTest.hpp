#ifndef CPE_DR_DATAPBUF_TEST_WRITETEST_H
#define CPE_DR_DATAPBUF_TEST_WRITETEST_H
#include <string.h>
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/dr/dr_pbuf.h"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) WriteTestBase;

class WriteTest : public testenv::fixture<WriteTestBase> {
public:
    WriteTest();
    virtual void SetUp();
    virtual void TearDown();

    struct mem_buffer m_buffer;

    LPDRMETALIB m_metaLib;
    struct mem_buffer m_metaLib_buffer;

    void installMeta(const char * data);

    int write(const void * data, size_t data_size, const char * typeName);
    const char * result(void);
};

#endif
