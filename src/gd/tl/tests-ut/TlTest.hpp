#ifndef GD_TL_TEST_TLTEST_H
#define GD_TL_TEST_TLTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "gd/tl/tl.h"
#include "../tl_internal_ops.h"

class TlTest : public testenv::fixture<> {
public:
    TlTest();

    virtual void SetUp();
    virtual void TearDown();

    gd_tl_manage_t m_manage;
    gd_tl_t m_tl;

    void installTl(void);

    gd_tl_event_t createEvent(size_t capacity);
    gd_tl_event_t createAction();
};

#endif
