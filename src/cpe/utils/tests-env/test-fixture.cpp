#include "cpe/utils/tests-env/test-fixture.hpp"

namespace testenv {

static void * do_alloc(size_t size, struct mem_allocrator * allocrator) {
    return mem_buffer_alloc((mem_buffer_t)(allocrator + 1), size);
}

void do_free(void * p, struct mem_allocrator * allocrator) {}

Test::Test() {
    m_allocrator.m_alloc = do_alloc;
    m_allocrator.m_free = do_free;
    mem_buffer_init(&m_alloc_buf, NULL);
}

Test::~Test() {
    mem_buffer_clear(&m_alloc_buf);
}

void Test::SetUp() {
}

void Test::TearDown() {
}

mem_allocrator_t
Test::t_allocrator() {
    return &m_allocrator;
}

void *
Test::t_alloc(size_t size) {
    return mem_buffer_alloc(&m_alloc_buf, size);
}

char *
Test::t_strdup(const char * str) {
    return mem_buffer_strdup(&m_alloc_buf, str);
}

}
