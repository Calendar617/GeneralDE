#include "CfgTest.hpp"

TEST_F(CfgTest, value_string) {
    gd_cfg_t cfg = gd_cfg_struct_add_string(m_root, "a", "test-str");
    ASSERT_TRUE(cfg);

    EXPECT_STREQ("test-str", gd_cfg_get_string(cfg, "", ""));
}

TEST_F(CfgTest, value_int8) {
    gd_cfg_t cfg = gd_cfg_struct_add_int8(m_root, "a", 3);
    ASSERT_TRUE(cfg);

    EXPECT_EQ(3, gd_cfg_get_int8(cfg, "", 0));
}

