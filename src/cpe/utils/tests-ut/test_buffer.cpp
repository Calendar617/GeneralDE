#include "BufferTest.hpp"

TEST_F(BufferTest, append_empty) {
    const char * data = "test_string";
    
    EXPECT_EQ(strlen(data), append_string(data));
    EXPECT_EQ((size_t)1, append_zero());

    EXPECT_EQ(strlen(data) + 1, mem_buffer_size(&m_buffer));

    EXPECT_STREQ(data, as_string());
}

TEST_F(BufferTest, append_multi_trunk) {
    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    append_zero();

    EXPECT_EQ((size_t)4, mem_buffer_size(&m_buffer));
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
    EXPECT_TRUE(mem_buffer_make_continuous(&m_buffer, 0) == NULL);
}

TEST_F(BufferTest, make_continuous_single_trunk) {
    char buf[5] = { 0 };

    struct mem_buffer_trunk * told = append_trunk("a");
    void * newDataAddr = mem_buffer_make_continuous(&m_buffer, 0);
    EXPECT_TRUE(told == trunk_at(0));

    memcpy(buf, newDataAddr, mem_buffer_size(&m_buffer));

    EXPECT_STREQ("a", buf);
}

TEST_F(BufferTest, make_continuous_single_trunk_with_capacity) {
    char buf[5] = { 0 };

    struct mem_buffer_trunk * told = append_trunk("a");
    void * newDataAddr = mem_buffer_make_continuous(&m_buffer, 1024);
    EXPECT_TRUE(told != trunk_at(0));
    EXPECT_EQ(1, mem_buffer_size(&m_buffer));

    memcpy(buf, newDataAddr, mem_buffer_size(&m_buffer));

    EXPECT_STREQ("a", buf);
}

TEST_F(BufferTest, make_continuous_multi_trunk) {
    char buf[5] = { 0 };

    EXPECT_TRUE(append_trunk("a"));
    EXPECT_TRUE(append_trunk("b"));
    EXPECT_TRUE(append_trunk("c"));

    memcpy(buf,mem_buffer_make_continuous(&m_buffer, 0), mem_buffer_size(&m_buffer));

    EXPECT_STREQ("abc", buf);
}

TEST_F(BufferTest, alloc_empty) {
    void * p = mem_buffer_alloc(&m_buffer, 12);
    EXPECT_TRUE(p);
    EXPECT_EQ((size_t)12, mem_buffer_size(&m_buffer));

    strncpy((char*)p, "abc", 12);
    EXPECT_STREQ("abc", as_string());
}

TEST_F(BufferTest, alloc_empty_null) {
    m_buffer.m_default_allocrator = mem_allocrator_null();
    EXPECT_FALSE(mem_buffer_alloc(&m_buffer, 12));
    EXPECT_EQ((size_t)0, mem_buffer_size(&m_buffer));
}

TEST_F(BufferTest, strdup_basic) {
    EXPECT_STREQ("abc", mem_buffer_strdup(&m_buffer, "abc"));
    EXPECT_EQ((size_t)4, mem_buffer_size(&m_buffer));
}

TEST_F(BufferTest, strndup_basic) {
    EXPECT_STREQ("abc", mem_buffer_strndup(&m_buffer, "abcd", 3));
    EXPECT_EQ((size_t)4, mem_buffer_size(&m_buffer));
}
