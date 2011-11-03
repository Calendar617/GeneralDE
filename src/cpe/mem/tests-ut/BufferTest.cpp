#include "BufferTest.hpp"

void BufferTest::SetUp() {
    mem_buffer_init(&m_buffer, 0);
}

void BufferTest::TearDown() {
    mem_buffer_free(&m_buffer);
}

int BufferTest::append_string(const char * data) {
    return mem_buffer_append(&m_buffer, data, strlen(data) + 1);
}

struct mem_buffer_trunk *
BufferTest::append_trunk(const char * data) {
    if (data) {
        size_t capacity = strlen(data) + 1;
        struct mem_buffer_trunk * trunk =
            mem_buffer_append_trunk(&m_buffer, capacity);
        mem_trunk_append(&m_buffer, trunk, data, capacity);
        return trunk;
    }
    else {
        return mem_buffer_append_trunk(&m_buffer, m_buffer.m_auto_inc_size);
    }
}

char *
BufferTest::as_string(void) {
    return (char *)mem_buffer_make_continuous(&m_buffer);
}
