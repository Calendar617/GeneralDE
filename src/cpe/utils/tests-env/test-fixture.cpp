#include "cpe/utils/tests-env/test-fixture.hpp"

namespace testenv {

static void * do_tmp_alloc(size_t size, struct mem_allocrator * allocrator) {
    return mem_buffer_alloc((mem_buffer_t)(allocrator + 1), size);
}

void do_tmp_free(void * p, struct mem_allocrator * allocrator) {
}

static void * do_alloc(size_t size, struct mem_allocrator * allocrator) {
    void * r = mem_buffer_alloc((mem_buffer_t)(allocrator + 1), size);

    ++*((int*)(((mem_buffer_t)(allocrator + 1)) + 1));

    return r;
}

void do_free(void * p, struct mem_allocrator * allocrator) {
    ++*(((int*)(((mem_buffer_t)(allocrator + 1)) + 1)) + 1);
}

Test::Test()
    : m_alloc_count(0)
    , m_free_count(0)
{
    m_tmp_allocrator.m_alloc = do_tmp_alloc;
    m_tmp_allocrator.m_free = do_tmp_free;
    mem_buffer_init(&m_tmp_alloc_buf, NULL);

    m_allocrator.m_alloc = do_alloc;
    m_allocrator.m_free = do_free;
    mem_buffer_init(&m_alloc_buf, NULL);
}

Test::~Test() {
    mem_buffer_clear(&m_tmp_alloc_buf);
    mem_buffer_clear(&m_alloc_buf);
}

int Test::t_alloc_count(void) const {
    return m_alloc_count;
}

int Test::t_free_count(void) const {
    return m_free_count;
}

void Test::SetUp() {
}

void Test::TearDown() {
    CHECK_NO_MEMLEAK();
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

mem_allocrator_t
Test::t_tmp_allocrator() {
    return &m_tmp_allocrator;
}

void *
Test::t_tmp_alloc(size_t size) {
    return mem_buffer_alloc(&m_tmp_alloc_buf, size);
}

char *
Test::t_tmp_strdup(const char * str) {
    return mem_buffer_strdup(&m_tmp_alloc_buf, str);
}

}
