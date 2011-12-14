#include "cpe/utils/hash_string.h"
#include "NmTest.hpp"

TEST_F(NmTest, instance_basic) {
    gd_nm_node_t instance = t_nm_add_instance("abc", 128);
    EXPECT_TRUE(instance);

    EXPECT_STREQ("abc", gd_nm_node_name(instance));
    EXPECT_EQ(gd_nm_node_instance, gd_nm_node_type(instance));
    EXPECT_EQ((size_t)128, gd_nm_node_capacity(instance));

    EXPECT_TRUE(instance == t_nm_find("abc"));
}

TEST_F(NmTest, instance_free) {
    gd_nm_node_t instance = t_nm_add_instance("abc", 128);
    EXPECT_TRUE(instance);

    gd_nm_node_free(instance);

    EXPECT_TRUE(NULL == t_nm_find("abc"));
}

TEST_F(NmTest, instance_it_multi_group) {
    t_nm_add_group("g1", 128);
    t_nm_add_group("g2", 128);
    t_nm_add_instance("i1", 128);

    EXPECT_EQ(0, t_nm_bind("g1", "i1"));
    EXPECT_EQ(0, t_nm_bind("g2", "i1"));

    EXPECT_STREQ("g2:g1:", t_nm_groups_of_node("i1").c_str());
}

TEST_F(NmTest, instance_it_empty) {
    t_nm_add_instance("i1", 128);

    EXPECT_STREQ("", t_nm_groups_of_node("i1").c_str());
}

