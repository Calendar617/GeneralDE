#include "SearchDirTest.hpp"

void SearchDirTest::SetUp() {
    Base::SetUp();

    m_stack_pos = 0;
    m_expect_stack_pos = 0;
}

static dir_visit_next_op_t
get_result(SearchDirTest * t) {
    if (t->m_stack_pos < t->m_expect_stack_pos) {
        return t->m_expect_stack[t->m_stack_pos++];
    }
    else {
        return dir_visit_next_exit;
    }
}

static dir_visit_next_op_t
on_dir_enter(const char * full, const char * base, void * ctx)
{
    SearchDirTest * t = (SearchDirTest *)ctx;
    t->m_result << ":enter(" << base << ")";
    return get_result(t);
}

static dir_visit_next_op_t
on_dir_leave(const char * full, const char * base, void * ctx) {
    SearchDirTest * t = (SearchDirTest *)ctx;
    t->m_result << ":leave(" << base << ")";
    return get_result(t);
}

static dir_visit_next_op_t
on_file(const char * full, const char * base, void * ctx) {
    SearchDirTest * t = (SearchDirTest *)ctx;
    t->m_result << ":file(" << base << ")";
    return get_result(t);
}

void
SearchDirTest::do_search(int maxLevel) {
    struct dir_visitor visitor;
    visitor.on_dir_enter = on_dir_enter;
    visitor.on_dir_leave = on_dir_leave;
    visitor.on_file = on_file;
    dir_search(&visitor, this, t_dir_base(), maxLevel, t_em(), t_allocrator());
}

void
SearchDirTest::expectFile(const char * file, dir_visit_next_op_t r) {
    ASSERT_TRUE(m_expect_stack_pos < 128);
    m_expect_stack[m_expect_stack_pos] = r;
    ++m_expect_stack_pos;
    m_expect << ":file(" << file << ")";
}

void
SearchDirTest::expectEnter(const char * file, dir_visit_next_op_t r) {
    ASSERT_TRUE(m_expect_stack_pos < 128);
    m_expect_stack[m_expect_stack_pos] = r;
    ++m_expect_stack_pos;
    m_expect << ":enter(" << file << ")";
}

void
SearchDirTest::expectLeave(const char * file, dir_visit_next_op_t r) {
    ASSERT_TRUE(m_expect_stack_pos < 128);
    m_expect_stack[m_expect_stack_pos] = r;
    ++m_expect_stack_pos;
    m_expect << ":leave(" << file << ")";
}

