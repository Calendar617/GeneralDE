#include "CfgTest.hpp"

class StructTest : public CfgTest {};

TEST_F(StructTest, add_basic) {
    cfg_t s = cfg_struct_add_struct(m_root, "a", cfg_replace);
    ASSERT_TRUE(s);
    EXPECT_STREQ("a", cfg_name(s));


    EXPECT_EQ(CPE_CFG_TYPE_STRUCT, cfg_type(s));
    EXPECT_TRUE(m_root == cfg_parent(s));
}

TEST_F(StructTest, add_seq) {
    cfg_t seq = cfg_struct_add_seq(m_root, "a", cfg_replace);
    ASSERT_TRUE(seq);

    EXPECT_EQ(CPE_CFG_TYPE_SEQUENCE, cfg_type(seq));
    EXPECT_TRUE(m_root == cfg_parent(seq));

    EXPECT_EQ(0, cfg_seq_count(seq));
}

TEST_F(StructTest, add_seq_seq_duplicate) {
    cfg_t seq = cfg_struct_add_seq(m_root, "a", cfg_replace);
    EXPECT_TRUE(seq);

    cfg_t newCfg = cfg_struct_add_seq(m_root, "a", cfg_replace);

    EXPECT_TRUE(seq == newCfg);
}

TEST_F(StructTest, add_not_from_struct) {
    cfg_t not_struct = cfg_struct_add_int8(m_root, "a", 3, cfg_replace);
    ASSERT_TRUE(not_struct);

    cfg_t s = cfg_struct_add_struct(not_struct, "a", cfg_replace);
    ASSERT_FALSE(s);
}

TEST_F(StructTest, find_empty) {
    EXPECT_FALSE(cfg_struct_find_cfg(m_root, "a"));
}

TEST_F(StructTest, find_success) {
    cfg_t s = cfg_struct_add_struct(m_root, "a", cfg_replace);
    ASSERT_TRUE(s);

    EXPECT_TRUE(s == cfg_struct_find_cfg(m_root, "a"));
}

TEST_F(StructTest, find_not_from_struct) {
    EXPECT_FALSE(cfg_struct_find_cfg(cfg_struct_add_seq(m_root, "a", cfg_replace), ""));
}

TEST_F(StructTest, free_child) {
    cfg_t s = cfg_struct_add_struct(m_root, "a", cfg_replace);
    ASSERT_TRUE(s);

    EXPECT_TRUE(s == cfg_struct_find_cfg(m_root, "a"));

    cfg_free(s);

    EXPECT_TRUE(NULL == cfg_struct_find_cfg(m_root, "a"));
}

