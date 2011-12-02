#include "ReadTest.hpp"

class PathTest : public ReadTest {
};

TEST_F(PathTest, map_root) {
    cfg_t cfg = cfg_find_cfg(m_root, "");
    ASSERT_TRUE(cfg == m_root);
}

TEST_F(PathTest, map_empty_name) {
    EXPECT_EQ(
        0, read(
            "'': abc\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, ".");
    ASSERT_TRUE(cfg);
    EXPECT_STREQ("", cfg_name(cfg));
    EXPECT_STREQ("abc", (const char *)cfg_data(cfg));
}

TEST_F(PathTest, map_basic) {
    EXPECT_EQ(
        0, read(
            "a: abc\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a");
    ASSERT_TRUE(cfg);
    EXPECT_STREQ("a", cfg_name(cfg));
    EXPECT_STREQ("abc", (const char *)cfg_data(cfg));
}

TEST_F(PathTest, map_not_exist) {
    EXPECT_EQ(
        0, read(
            "a: abc\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "c");
    ASSERT_FALSE(cfg);
}

TEST_F(PathTest, map_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "   b: abc"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a.b");
    ASSERT_TRUE(cfg);
    EXPECT_STREQ("b", cfg_name(cfg));
}

TEST_F(PathTest, map_map_not_exist) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "   b: abc"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a.c");
    ASSERT_FALSE(cfg);
}

TEST_F(PathTest, map_seq) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "   - abc"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0]");
    ASSERT_TRUE(cfg);
    EXPECT_STREQ("abc", (const char *)cfg_data(cfg));
}

TEST_F(PathTest, map_seq_seq) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "   - \n"
            "      - 123"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0][0]");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}
TEST_F(PathTest, map_seq_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "   - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0].b");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}

TEST_F(PathTest, map_seq_seq_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "    -\n"
            "       - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0][0].b");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}

TEST_F(PathTest, map_noname_seq_seq_map) {
    EXPECT_EQ(
        0, read(
            "-\n"
            "   - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "[0][0].b");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}

TEST_F(PathTest, map_seq_not_close) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0.b");
    ASSERT_FALSE(cfg);
}

TEST_F(PathTest, map_seq_pos_format_error) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[aa].b");
    ASSERT_FALSE(cfg);
}

TEST_F(PathTest, map_seq_name_sep_in) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a[0.0].b");
    ASSERT_FALSE(cfg);
}

TEST_F(PathTest, map_name_with_space) {
    EXPECT_EQ(
        0, read(
            "a c:\n"
            "  - b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(m_root, "a c[0].b");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}

TEST_F(PathTest, seq_root_map) {
    cfg_t r = cfg_struct_add_seq(m_root, "xxx");

    EXPECT_EQ(
        0, read(
            r,
            "b: 123\n"
            ));

    cfg_t cfg = cfg_find_cfg(r, "[0].b");
    ASSERT_TRUE(cfg);
    EXPECT_EQ(123, cfg_as_int32(cfg, -1));
}
