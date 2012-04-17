#ifndef CPE_DP_TEST_REQTEST_H
#define CPE_DP_TEST_REQTEST_H
#include "cpe/dp/dp_request.h"
#include "NodeTest.hpp"

typedef Loki::NullType ReqTestBase;

class ReqTest : public testenv::fixture<ReqTestBase, NodeTest> {
public:
    ReqTest();

    dp_req_t createReq(const char * type, size_t capacity);
    virtual void TearDown();

    dp_req_t m_req;
};

#endif
