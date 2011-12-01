#include "CfgTest.hpp"

class StructTest : public CfgTest {};

TEST_F(StructTest, add_basic) {
    gd_cfg_t s = gd_cfg_struct_add_struct(m_root, "a");
    ASSERT_TRUE(s);
    EXPECT_STREQ("a", gd_cfg_name(s));


    EXPECT_EQ(GD_CFG_TYPE_STRUCT, gd_cfg_type(s));
    EXPECT_TRUE(m_root == gd_cfg_parent(s));
}

TEST_F(StructTest, add_seq) {
    gd_cfg_t seq = gd_cfg_struct_add_seq(m_root, "a");
    ASSERT_TRUE(seq);

    EXPECT_EQ(GD_CFG_TYPE_SEQUENCE, gd_cfg_type(seq));
    EXPECT_TRUE(m_root == gd_cfg_parent(seq));

    EXPECT_EQ(0, gd_cfg_seq_count(seq));
}

TEST_F(StructTest, add_not_from_struct) {
    gd_cfg_t not_struct = gd_cfg_struct_add_int8(m_root, "a", 3);
    ASSERT_TRUE(not_struct);

    gd_cfg_t s = gd_cfg_struct_add_struct(not_struct, "a");
    ASSERT_FALSE(s);
}

TEST_F(StructTest, find_empty) {
    EXPECT_FALSE(gd_cfg_struct_find_cfg(m_root, "a"));
}

TEST_F(StructTest, find_success) {
    gd_cfg_t s = gd_cfg_struct_add_struct(m_root, "a");
    ASSERT_TRUE(s);

    EXPECT_TRUE(s == gd_cfg_struct_find_cfg(m_root, "a"));
}

TEST_F(StructTest, find_not_from_struct) {
    EXPECT_FALSE(gd_cfg_struct_find_cfg(gd_cfg_struct_add_seq(m_root, "a"), ""));
}

TEST_F(StructTest, free_child) {
    gd_cfg_t s = gd_cfg_struct_add_struct(m_root, "a");
    ASSERT_TRUE(s);

    EXPECT_TRUE(s == gd_cfg_struct_find_cfg(m_root, "a"));

    gd_cfg_free(s);

    EXPECT_TRUE(NULL == gd_cfg_struct_find_cfg(m_root, "a"));
}

