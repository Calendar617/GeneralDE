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

    struct mem_allocrator_count {
        struct mem_allocrator m_alloc;
        mem_allocrator_t m_parent_alloc;
        int m_alloc_count;
        int m_free_count;
    };

private:
    struct mem_allocrator m_tmp_allocrator;
    struct mem_buffer m_tmp_alloc_buf;

    struct mem_allocrator_count m_allocrator;
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

template <class Head>
struct fixture_gen<Loki::Typelist<Head, Loki::NullType> >
    : public Head
{
    void SetUp() {
        Head::SetUp();
    }

    void TearDown() {
        Head::TearDown();
    }
};

template<
    typename EnvListT = Loki::NullType
    , typename BaseT = Test
    , bool isNull = Loki::IsSameType<EnvListT, Loki::NullType>::value
>
class fixture;

template<typename EnvListT, typename BaseT>
class fixture<EnvListT, BaseT, false> : public BaseT, public fixture_gen<EnvListT> {
public:
    typedef fixture Base;

    virtual void SetUp() {
        BaseT::SetUp();
        fixture_gen<EnvListT>::SetUp();
    }

    virtual void TearDown() {
        fixture_gen<EnvListT>::TearDown();
        BaseT::TearDown();
    }

    using BaseT::t_allocrator;
    using BaseT::t_alloc;
    using BaseT::t_strdup;
};

template<typename EnvListT, typename BaseT>
class fixture<EnvListT, BaseT, true> : public BaseT {
public:
    typedef fixture Base;

    virtual void SetUp() {
        BaseT::SetUp();
    }

    virtual void TearDown() {
        BaseT::TearDown();
    }

    using BaseT::t_allocrator;
    using BaseT::t_alloc;
    using BaseT::t_strdup;
};

}

#define CHECK_NO_MEMLEAK()                                              \
    EXPECT_EQ(this->t_alloc_count(), this->t_free_count())              \
    << "allocrator use error!"

#endif
