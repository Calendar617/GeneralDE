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

TEST_F(ReadTest, root_map_map_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  b:\n"
            "     c: def\n"
            "d: gh\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    b:\n"
        "        c: def\n"
        "d: gh\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_seq) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - 1\n"
            "  - 2\n"
            "b:\n"
            "  - 3\n"
            "  - 4\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 1\n"
        "- 2\n"
        "b:\n"
        "- 3\n"
        "- 4\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_map_seq_map) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - b1: 1\n"
            "    b2: 2\n"
            "  - b3: 3\n"
            "    b4: 4\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "-   b1: 1\n"
        "    b2: 2\n"
        "-   b3: 3\n"
        "    b4: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_seq) {
    EXPECT_EQ(
        0, read(
            "- 1\n"
            "- 2\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "'':\n"
        "- 1\n"
        "- 2\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_seq_seq) {
    EXPECT_EQ(
        0, read(
            "-\n"
            "  - 1.1\n"
            "  - 1.2\n"
            "-\n"
            "  - 2.1\n"
            "  - 2.2\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "'':\n"
        "-   - 1.1\n"
        "    - 1.2\n"
        "-   - 2.1\n"
        "    - 2.2\n"
        "...\n"
        , result());
}

TEST_F(ReadTest, root_seq_map_seq) {
    EXPECT_EQ(
        0, read(
            "- a:\n"
            "  - 1.1\n"
            "  - 1.2\n"
            "- b:\n"
            "  - 2.1\n"
            "  - 2.2\n"
            ));

    EXPECT_STREQ(
        "---\n"
        "'':\n"
        "-   a:\n"
        "    - 1.1\n"
        "    - 1.2\n"
        "-   b:\n"
        "    - 2.1\n"
        "    - 2.2\n"
        "...\n"
        , result());
}
