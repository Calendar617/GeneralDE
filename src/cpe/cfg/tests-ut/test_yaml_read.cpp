#include "ReadTest.hpp"

TEST_F(ReadTest, root_map_basic) {
    EXPECT_EQ(
        0, read(
            "a: abc\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a: abc\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_two_item) {
    EXPECT_EQ(
        0, read(
            "a: abc\n"
            "b: abc\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a: abc\n"
        "b: abc\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  b: abc\n"
            "c: def\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    b: abc\n"
        "c: def\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_map_multi_item) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  b: abc\n"
            "  c: def\n"
            "d: gh\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    b: abc\n"
        "    c: def\n"
        "d: gh\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_to_parent) {
    EXPECT_EQ(
        -1, read(
            "    a: abc\n"
            "b: abc\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a: abc\n"
        "...\n"
        , result());
}
