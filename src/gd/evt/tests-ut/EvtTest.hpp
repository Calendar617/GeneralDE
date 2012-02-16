#ifndef GD_EVT_TEST_EVTTEST_H
#define GD_EVT_TEST_EVTTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/utils/buffer.h"
#include "gd/evt/evt_manage.h"
#include "gd/evt/evt_read.h"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) EvtTestBase;

class EvtTest : public testenv::fixture<EvtTestBase> {
public:
    EvtTest();

    virtual void SetUp();
    virtual void TearDown();

    void createEvtMgr(const char * metalib);

    gd_evt_t createEvt(const char * typeName, size_t attach_capacity = 0, ssize_t data_capacity = -1);

    gd_tl_manage_t m_tl_mgr;
    struct mem_buffer m_buffer;
    gd_evt_mgr_t m_evt_mgr;
};

#endif
