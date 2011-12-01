#include "CfgTest.hpp"

TEST_F(CfgTest, name_of_root) {
    EXPECT_STREQ("", gd_cfg_name(m_root));
}

TEST_F(CfgTest, name_of_struct_item) {
    EXPECT_STREQ(
        "aa",
        gd_cfg_name(
            gd_cfg_struct_add_struct(m_root, "aa")));
}

TEST_F(CfgTest, name_of_seq_item) {
    EXPECT_STREQ(
        "aa",
        gd_cfg_name(
            gd_cfg_seq_add_int8(
                gd_cfg_struct_add_seq(m_root, "aa")
                , 3)));
}

TEST_F(CfgTest, data_of_struct) {
    EXPECT_TRUE(NULL == gd_cfg_data(m_root));
}

TEST_F(CfgTest, data_of_sequence) {
    EXPECT_TRUE(NULL == gd_cfg_data(gd_cfg_struct_add_seq(m_root, "a")));
}

TEST_F(CfgTest, is_value_of_struct) {
    EXPECT_FALSE(gd_cfg_is_value(m_root));
}

TEST_F(CfgTest, is_value_of_sequence) {
    EXPECT_FALSE(gd_cfg_is_value(gd_cfg_struct_add_seq(m_root, "a")));
}

TEST_F(CfgTest, is_value_of_int) {
    EXPECT_TRUE(gd_cfg_is_value(gd_cfg_struct_add_int8(m_root, "a", 0)));
}

