#ifndef GD_DP_TEST_REQTEST_H
#define GD_DP_TEST_REQTEST_H
#include "gd/dp/dp_request.h"
#include "DpTest.hpp"

typedef Loki::NullType ReqTestBase;

class ReqTest : public testenv::fixture<ReqTestBase, DpTest> {
public:
    ReqTest();

    gd_dp_req_t createReq(const char * type, size_t capacity);
    virtual void TearDown();

    gd_dp_req_t m_req;
};

#endif
