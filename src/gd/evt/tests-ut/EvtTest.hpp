#ifndef GD_EVT_TEST_EVTTEST_H
#define GD_EVT_TEST_EVTTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "cpe/utils/buffer.h"
#include "gd/app/tests-env/with_app.hpp"
#include "gd/dr_store/tests-env/with_dr_store.hpp"
#include "gd/evt/evt_manage.h"
#include "gd/evt/evt_read.h"

typedef LOKI_TYPELIST_3(
    utils::testenv::with_em,
    gd::app::testenv::with_app,
    gd::dr_store::testenv::with_dr_store) EvtTestBase;

class EvtTest : public testenv::fixture<EvtTestBase> {
public:
    EvtTest();

    virtual void SetUp();
    virtual void TearDown();

    void createEvtMgr(const char * metalib);

    gd_evt_t createEvt(const char * typeName, size_t carry_size = 0, ssize_t data_capacity = -1);

    tl_manage_t m_tl_mgr;
    struct mem_buffer m_buffer;
    gd_evt_mgr_t m_evt_mgr;
};

#endif
