#include "CfgTest.hpp"

TEST_F(CfgTest, value_string) {
    cfg_t cfg = cfg_struct_add_string(m_root, "a", "test-str");
    ASSERT_TRUE(cfg);

    EXPECT_STREQ("test-str", cfg_get_string(cfg, "", ""));
}

TEST_F(CfgTest, value_int8) {
    cfg_t cfg = cfg_struct_add_int8(m_root, "a", 3);
    ASSERT_TRUE(cfg);

    EXPECT_EQ(3, cfg_get_int8(cfg, "", 0));
}

