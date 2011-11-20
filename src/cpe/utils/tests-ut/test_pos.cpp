#include "BufferTest.hpp"

TEST_F(BufferTest, pos_valid_head) {
    append_string("a");

    struct mem_buffer_pos l;
    mem_buffer_begin(&l, &m_buffer);

    EXPECT_EQ(1, mem_pos_valide(&l));
}

TEST_F(BufferTest, pos_valid_end) {
    append_string("a");

    struct mem_buffer_pos l;
    mem_buffer_end(&l, &m_buffer);

    EXPECT_EQ(0, mem_pos_valide(&l));
}

TEST_F(BufferTest, pos_begin_end_ne) {
    append_string("a");

    struct mem_buffer_pos l;
    mem_buffer_begin(&l, &m_buffer);

    struct mem_buffer_pos r;
    mem_buffer_end(&r, &m_buffer);
    
    EXPECT_EQ(0, mem_pos_eq(&l, &r));
}

TEST_F(BufferTest, pos_begin_end_eq) {
    struct mem_buffer_pos l;
    mem_buffer_begin(&l, &m_buffer);

    struct mem_buffer_pos r;
    mem_buffer_end(&r, &m_buffer);

    EXPECT_EQ(1, mem_pos_eq(&l, &r));
}

TEST_F(BufferTest, pos_at) {
    append_string("abc");

    struct mem_buffer_pos p;
    mem_pos_at(&p, &m_buffer, 0);
    EXPECT_EQ('a', mem_pos_data(&p));

    mem_pos_at(&p, &m_buffer, 1);
    EXPECT_EQ('b', mem_pos_data(&p));

    mem_pos_at(&p, &m_buffer, 2);
    EXPECT_EQ('c', mem_pos_data(&p));

    mem_pos_at(&p, &m_buffer, 3);
    EXPECT_FALSE(mem_pos_valide(&p));
}

TEST_F(BufferTest, pos_seek_fwd_basic) {
    struct mem_buffer_trunk * t1 = append_trunk("abc");

    struct mem_buffer_pos p;
    mem_buffer_begin(&p, &m_buffer);
    EXPECT_EQ(1, mem_pos_seek(&p, 1));

    EXPECT_TRUE(t1 == p.m_trunk);
    EXPECT_EQ((size_t)1, p.m_pos_in_trunk);
}

TEST_F(BufferTest, pos_seek_fwd_to_last) {
    struct mem_buffer_trunk * t1 = append_trunk("abc");

    struct mem_buffer_pos p;
    mem_buffer_begin(&p, &m_buffer);
    EXPECT_EQ(2, mem_pos_seek(&p, 2));

    EXPECT_TRUE(t1 == p.m_trunk);
    EXPECT_EQ((size_t)2, p.m_pos_in_trunk);
}
