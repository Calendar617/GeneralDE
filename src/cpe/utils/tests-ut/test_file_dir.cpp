#include "FileTest.hpp"

TEST_F(FileTest, dir_mk_recursion_basic) {
    EXPECT_EQ(
        0,
        dir_mk_recursion(
            make_path_name("a/b"),
            DIR_DEFAULT_MODE,
            &m_em, NULL));

    EXPECT_TRUE(dir_exist(make_path_name("a/b")));
}

TEST_F(FileTest, dir_mk_recursion_one_level) {
    EXPECT_EQ(
        0,
        dir_mk_recursion(
            make_path_name("a"),
            DIR_DEFAULT_MODE,
            &m_em, NULL));

    EXPECT_TRUE(dir_exist(make_path_name("a")));
}

TEST_F(FileTest, dir_mk_recursion_empty) {
    EXPECT_EQ(
        -1,
        dir_mk_recursion(
            "",
            DIR_DEFAULT_MODE,
            &m_em, NULL));
}

TEST_F(FileTest, dir_exist_basic) {
    EXPECT_EQ(
        1,
        dir_exist(make_path_name(".")));
}

TEST_F(FileTest, dir_exist_not_exist) {
    EXPECT_EQ(
        0,
        dir_exist(make_path_name("not-exist")));
}

TEST_F(FileTest, dir_exist_file) {
    EXPECT_EQ(
        0,
        file_write_from_str(
            make_path_name("f1"),
            "",
            &m_em));

    EXPECT_EQ(
        0,
        dir_exist(make_path_name("not-exist")));
}
