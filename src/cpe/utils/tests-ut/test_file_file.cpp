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

    struct mem_buffer buffer;
    mem_buffer_init(&buffer, NULL);

    EXPECT_EQ(3, file_load_to_buffer(&buffer, make_path_name("a.txt"), &m_em));
    mem_buffer_append_char(&buffer, 0);

    EXPECT_STREQ(
        "abc",
        (const char *)mem_buffer_make_exactly(&buffer));

    mem_buffer_clear(&buffer);
}

