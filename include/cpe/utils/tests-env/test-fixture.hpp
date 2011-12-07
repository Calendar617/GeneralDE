#ifndef CPE_UTILS_TEST_FIXTURE_H
#define CPE_UTILS_TEST_FIXTURE_H
#include "gtest/gtest.h"
#include "loki/Typelist.h"
#include "loki/TypelistMacros.h"
#include "../memory.h"
#include "../buffer.h"

namespace testenv {

class Test : public ::testing::Test {
public:
    Test();
    ~Test();

    virtual void SetUp();
    virtual void TearDown();


    mem_allocrator_t t_tmp_allocrator();
    void * t_tmp_alloc(size_t size);
    char * t_tmp_strdup(const char * str);

    mem_allocrator_t t_allocrator();
    void * t_alloc(size_t size);
    char * t_strdup(const char * str);

    int t_alloc_count(void) const;
    int t_free_count(void) const;
private:
    struct mem_allocrator m_tmp_allocrator;
    struct mem_buffer m_tmp_alloc_buf;
    struct mem_allocrator m_allocrator;
    struct mem_buffer m_alloc_buf;
    int m_alloc_count;
    int m_free_count;
};

template <typename TList> struct fixture_gen;

template <class Head, class Tail>
struct fixture_gen<Loki::Typelist<Head, Tail> >
    : public Head
    , public fixture_gen<Tail>
{
    void SetUp() {
        Head::SetUp();
        fixture_gen<Tail>::SetUp();
    }

    void TearDown() {
        fixture_gen<Tail>::TearDown();
        Head::TearDown();
    }
};

template <>
struct fixture_gen<Loki::NullType> {
    void SetUp() {}
    void TearDown() {}
};

template<typename EnvListT = Loki::NullType, typename BaseT = Test>
class fixture : public BaseT, public fixture_gen<EnvListT> {
public:
    typedef fixture Base;

    virtual void SetUp() {
        fixture_gen<EnvListT>::SetUp();
        BaseT::SetUp();
    }

    virtual void TearDown() {
        BaseT::TearDown();
        fixture_gen<EnvListT>::TearDown();
    }

    using BaseT::t_allocrator;
    using BaseT::t_alloc;
    using BaseT::t_strdup;
};

}

#define CHECK_NO_MEMLEAK()                                              \
    EXPECT_TRUE(this->t_alloc_count() == this->t_free_count())          \
    << "allocrator use error! alloc count is " << this->t_alloc_count() \
    << ", free count is " << t_free_count()

#endif
