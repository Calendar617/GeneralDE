#include "cpe/utils/tests-env/test-fixture.hpp"

namespace testenv {

static void * do_tmp_alloc(size_t size, struct mem_allocrator * allocrator) {
    return mem_buffer_alloc((mem_buffer_t)(allocrator + 1), size);
}

void do_tmp_free(void * p, struct mem_allocrator * allocrator) {
}

static void * do_count_alloc(size_t size, struct mem_allocrator * allocrator) {
    struct Test::mem_allocrator_count * count_alloc = (struct Test::mem_allocrator_count *)allocrator;
    ++count_alloc->m_alloc_count;
    return mem_alloc(count_alloc->m_parent_alloc, size);
}

void do_count_free(void * p, struct mem_allocrator * allocrator) {
    struct Test::mem_allocrator_count * count_alloc = (struct Test::mem_allocrator_count *)allocrator;
    ++count_alloc->m_free_count;
    return mem_free(count_alloc->m_parent_alloc, p);
}

Test::Test() {
    m_tmp_allocrator.m_alloc = do_tmp_alloc;
    m_tmp_allocrator.m_free = do_tmp_free;
    mem_buffer_init(&m_tmp_alloc_buf, NULL);

    m_allocrator.m_alloc.m_alloc = do_count_alloc;
    m_allocrator.m_alloc.m_free = do_count_free;
    m_allocrator.m_parent_alloc = NULL;
    m_allocrator.m_alloc_count = 0;
    m_allocrator.m_free_count = 0;
}

Test::~Test() {
    mem_buffer_clear(&m_tmp_alloc_buf);
}

int Test::t_alloc_count(void) const {
    return m_allocrator.m_alloc_count;
}

int Test::t_free_count(void) const {
    return m_allocrator.m_free_count;
}

void Test::SetUp() {
}

void Test::TearDown() {
    CHECK_NO_MEMLEAK();
}

mem_allocrator_t
Test::t_allocrator() {
    return &m_allocrator.m_alloc;
}

void *
Test::t_alloc(size_t size) {
    return mem_alloc(t_allocrator(), size);
}

char *
Test::t_strdup(const char * str) {
    size_t len = strlen(str) + 1;
    char * p = (char*)t_alloc(len);
    memcpy(p, str, len);
    return p;
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
