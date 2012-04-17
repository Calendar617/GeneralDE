#ifndef CPE_DP_TEST_NODETEST_H
#define CPE_DP_TEST_NODETEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/dp/dp_node.h"
#include "cpe/nm/tests-env/with_nm.hpp"
#include "DpTest.hpp"

typedef LOKI_TYPELIST_1(
    gd::nm::testenv::with_nm) NodeTestBase;

class NodeTest : public testenv::fixture<NodeTestBase, DpTest> {
public:
};

#endif
