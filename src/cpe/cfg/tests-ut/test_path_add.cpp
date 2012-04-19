#include "CfgTest.hpp"

class PathWriteTest : public CfgTest {
};

TEST_F(PathWriteTest, struct_basic) {
    cfg_t cfg = cfg_add_struct(m_root, "a", t_em());

    EXPECT_TRUE(cfg);

    EXPECT_STREQ(
        "---\n"
        "a: {}\n"
        "...\n"
        , result());
}

TEST_F(PathWriteTest, seq_basic) {
    cfg_t cfg = cfg_add_struct(m_root, "a[]", t_em());

    EXPECT_TRUE(cfg);

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- {}\n"
        "...\n"
        , result());
}

