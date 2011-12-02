#include "ReadTest.hpp"

class ReadMergeTest : public ReadTest {};

TEST_F(ReadMergeTest, scalar_to_scalar___replace) {
    EXPECT_EQ(
        0, read(
            "a: 1\n"
            "b: 2\n"
            "b: 3\n"
            ,
            cfg_read_replace
            ));

    EXPECT_STREQ(
        "---\n"
        "a: 1\n"
        "b: 3\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, scalar_to_scalar___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a: 1\n"
            "b: 2\n"
            "b: 3\n"
            ,
            cfg_read_merge_mine
            ));

    EXPECT_STREQ(
        "---\n"
        "a: 1\n"
        "b: 3\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, scalar_to_scalar_merge_yours) {
    EXPECT_EQ(
        0, read(
            "a: 1\n"
            "b: 2\n"
            "b: 3\n"
            ,
            cfg_read_merge_yours
            ));

    EXPECT_STREQ(
        "---\n"
        "a: 1\n"
        "b: 2\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, map_to_map___replace) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  a3: 3\n"
            "c: 4\n"
            ,
            cfg_read_replace));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a3: 3\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, map_to_map___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  a2: 3\n"
            "  a3: 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_mine));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a1: 1\n"
        "    a2: 3\n"
        "    a3: 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, map_to_map___merge_yours) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  a2: 3\n"
            "  a3: 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_yours));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a1: 1\n"
        "    a2: 2\n"
        "    a3: 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, map_to_schela___merge_yours) {
    EXPECT_EQ(
        0, read(
            "a: 1\n"
            "b: 3\n"
            "a:\n"
            "  a2: 3\n"
            "  a3: 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_yours));

    EXPECT_STREQ(
        "---\n"
        "a: 1\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, map_to_schela___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a: 1\n"
            "b: 3\n"
            "a:\n"
            "  a2: 3\n"
            "  a3: 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_mine));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a2: 3\n"
        "    a3: 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_seq___replace) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - 1\n"
            "  - 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_replace));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 3\n"
        "- 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_seq___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - 1\n"
            "  - 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_mine));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 1\n"
        "- 2\n"
        "- 3\n"
        "- 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_seq___merge_yours) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  - 1\n"
            "  - 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_yours));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 1\n"
        "- 2\n"
        "- 3\n"
        "- 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_map___replace) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_replace));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 3\n"
        "- 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_map___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_mine));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "- 3\n"
        "- 4\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, seq_to_map___merge_yours) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a:\n"
            "  - 3\n"
            "  - 4\n"
            "c: 4\n"
            ,
            cfg_read_merge_yours));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a1: 1\n"
        "    a2: 2\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, null___replace) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a: !!null\n"
            "c: 4\n"
            ,
            cfg_read_replace));

    EXPECT_STREQ(
        "---\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, null___merge_mine) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a: !!null\n"
            "c: 4\n"
            ,
            cfg_read_merge_mine));

    EXPECT_STREQ(
        "---\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}

TEST_F(ReadMergeTest, null___merge_yours) {
    EXPECT_EQ(
        0, read(
            "a:\n"
            "  a1: 1\n"
            "  a2: 2\n"
            "b: 3\n"
            "a: !!null\n"
            "c: 4\n"
            ,
            cfg_read_merge_yours));

    EXPECT_STREQ(
        "---\n"
        "a:\n"
        "    a1: 1\n"
        "    a2: 2\n"
        "b: 3\n"
        "c: 4\n"
        "...\n"
        , result());
}
