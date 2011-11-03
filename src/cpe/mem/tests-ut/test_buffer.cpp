#include "BufferTest.hpp"

TEST_F(BufferTest, append_empty) {
    const char * data = "test_string";
    
    EXPECT_EQ(0, append_string(data));
    EXPECT_EQ(strlen(data) + 1, mem_buffer_size(&m_buffer));

    EXPECT_STREQ(data, as_string());
}

TEST_F(BufferTest, append_multi_trunk) {
    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    EXPECT_EQ(6, mem_buffer_size(&m_buffer));
}
