#include <string.h>
#include "NodeTest.hpp"

static struct gd_dp_node_type type = {
    "test-type",
    NULL,
    NULL
};

TEST_F(NodeTest, node_create_basic) {
    gd_dp_node_t node = gd_dp_node_create(t_nm(), "node1", &type, 12);
    // ASSERT_TRUE(node);

    // strcpy((char *)gd_dp_node_data(node), "abcd");

    // EXPECT_STREQ("node1", gd_dp_node_name(node));
    // EXPECT_TRUE(&type == gd_dp_node_type(node));
    // EXPECT_EQ((size_t)12, gd_dp_node_capacity(node));
    // EXPECT_STREQ("abcd", (char *)gd_dp_node_data(node));
}

