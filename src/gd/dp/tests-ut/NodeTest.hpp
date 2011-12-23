#ifndef GD_DP_TEST_NODETEST_H
#define GD_DP_TEST_NODETEST_H
#include "gd/dp/dp_node.h"
#include "gd/nm/tests-env/with_nm.hpp"
#include "DpTest.hpp"

typedef LOKI_TYPELIST_1(
    gd::nm::testenv::with_nm) NodeTestBase;

class NodeTest : public testenv::fixture<NodeTestBase, DpTest> {
public:
};

#endif
