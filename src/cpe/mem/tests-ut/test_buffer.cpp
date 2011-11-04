#include "BufferTest.hpp"

TEST_F(BufferTest, append_empty) {
    const char * data = "test_string";
    
    EXPECT_EQ(0, append_string(data));
    EXPECT_EQ(0, append_zero());

    EXPECT_EQ(strlen(data) + 1, mem_buffer_size(&m_buffer));

    EXPECT_STREQ(data, as_string());
}

TEST_F(BufferTest, append_multi_trunk) {
    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    append_zero();

    EXPECT_EQ(4, mem_buffer_size(&m_buffer));
    EXPECT_STREQ("abc", as_string());
}

TEST_F(BufferTest, read_from_empty) {
    char buf[5];

    EXPECT_EQ(0, mem_buffer_read(buf, 5, &m_buffer));
}

TEST_F(BufferTest, read_basic) {
    char buf[5];

    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    EXPECT_EQ(3, mem_buffer_read(buf, 5, &m_buffer));
    buf[3] = 0;

    EXPECT_STREQ("abc", buf);
}

TEST_F(BufferTest, make_continuous_no_trunk) {
    EXPECT_TRUE(mem_buffer_make_continuous(&m_buffer) == NULL);
}

TEST_F(BufferTest, make_continuous_single_trunk) {
    char buf[5] = { 0 };

    EXPECT_TRUE(append_trunk("a"));

    memcpy(buf,mem_buffer_make_continuous(&m_buffer), mem_buffer_size(&m_buffer));

    EXPECT_STREQ("a", buf);
}

TEST_F(BufferTest, make_continuous_multi_trunk) {
    char buf[5] = { 0 };

    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    memcpy(buf,mem_buffer_make_continuous(&m_buffer), mem_buffer_size(&m_buffer));

    EXPECT_STREQ("abc", buf);
}
