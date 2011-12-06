#include "FileTest.hpp"

TEST_F(FileTest, file_write_from_buf_basic) {
    const char * data = "abc";

    EXPECT_EQ(
        3, 
        file_write_from_buf(
            make_path_name("a.txt"),
            data,
            3,
            &m_em));

    EXPECT_STREQ("abc", file_as_str("a.txt"));
}

TEST_F(FileTest, file_write_from_string_basic) {
    const char * data = "abc";

    EXPECT_EQ(
        3, 
        file_write_from_str(
            make_path_name("a.txt"),
            data,
            &m_em));

    EXPECT_STREQ("abc", file_as_str("a.txt"));
}

TEST_F(FileTest, file_size_basic) {
    write_file("a.txt", "abc");

    EXPECT_EQ(
        3, 
        file_size(make_path_name("a.txt"), &m_em));
}

TEST_F(FileTest, file_size_not_exist) {
    EXPECT_EQ(
        -1, 
        file_size(make_path_name("a.txt"), &m_em));
}

TEST_F(FileTest, file_stream_size) {
    write_file("a.txt", "abc");

    FILE * fp = file_stream_open(make_path_name("a.txt"), "r", &m_em);
    ASSERT_TRUE(fp);

    EXPECT_EQ(
        3, 
        file_stream_size(fp, &m_em));
}

TEST_F(FileTest, file_append_from_buf_basic) {
    write_file("a.txt", "abc");

    EXPECT_EQ(
        3, 
        file_append_from_buf(
            make_path_name("a.txt"),
            "def",
            3,
            &m_em));

    EXPECT_STREQ("abcdef", file_as_str("a.txt"));
}

TEST_F(FileTest, file_append_from_string_basic) {
    write_file("a.txt", "abc");

    EXPECT_EQ(
        3, 
        file_append_from_str(
            make_path_name("a.txt"),
            "def",
            &m_em));

    EXPECT_STREQ("abcdef", file_as_str("a.txt"));
}
