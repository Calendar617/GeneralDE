#ifndef CPE_DR_TEST_SEARCHDIRTEST_H
#define CPE_DR_TEST_SEARCHDIRTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_file.hpp"
#include "cpe/utils/tests-env/with_em.hpp"

typedef LOKI_TYPELIST_2(
    utils::testenv::with_file
    , utils::testenv::with_em) SearchDirTestBase;

class SearchDirTest : public testenv::fixture<SearchDirTestBase> {
public:
    virtual void SetUp();

    void do_search(int maxLevel);
    void expectFile(const char * file, dir_visit_next_op_t r);
    void expectEnter(const char * file, dir_visit_next_op_t r);
    void expectLeave(const char * file, dir_visit_next_op_t r);

    dir_visit_next_op_t m_expect_stack[128]; 
    int m_expect_stack_pos;
    int m_stack_pos;

    ::std::ostringstream m_expect;
    ::std::ostringstream m_result;
};

#endif

