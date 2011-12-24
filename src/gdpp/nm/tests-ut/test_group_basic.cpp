#include <stdexcept>
#include "GroupTest.hpp"

TEST_F(GroupTest, create_basic) {
    TestGroup * o = new(t_nm(), "object1") TestGroup(*this, 1);
    ASSERT_TRUE(o);

    EXPECT_STREQ("object1", o->name());
    EXPECT_EQ(gd_nm_node_group, o->category());
}

TEST_F(GroupTest, create_duplicate) {
    TestGroup * g1 = new(t_nm(), "g1") TestGroup(*this, 1);
    ASSERT_TRUE(g1);

    EXPECT_THROW(
        new(t_nm(), "g1") TestGroup(*this, 1)
        , ::std::runtime_error);
}

class ExceptionCreateGroup : public GroupTest::TestGroup {
public:
    ExceptionCreateGroup(GroupTest & t) : TestGroup(t, 0) {
        throw "exception";
    }
};

TEST_F(GroupTest, create_exception) {
    EXPECT_THROW(
        new(t_nm(), "object1") ExceptionCreateGroup(*this),
        const char *);

    EXPECT_EQ(1, _destoryCount);
}

TEST_F(GroupTest, destory_by_delete) {
    TestGroup * o = new(t_nm(), "object1") TestGroup(*this, 1);
    ASSERT_TRUE(o);

    delete o;

    EXPECT_EQ(1, _destoryCount);
}

TEST_F(GroupTest, destory_by_node) {
    TestGroup * o = new(t_nm(), "object1") TestGroup(*this, 1);
    ASSERT_TRUE(o);

    gd_nm_node_t node = gd_nm_mgr_find_node(t_nm(), o->name_hs());
    ASSERT_TRUE(node);
    gd_nm_node_free(node);

    EXPECT_EQ(1, _destoryCount);
}

TEST_F(GroupTest, addMember_basic) {
    TestGroup * g1 = new(t_nm(), "g1") TestGroup(*this, 1);
    ASSERT_TRUE(g1);

    TestObject * i1 = new(t_nm(), "o1") TestObject(*this, 1);
    ASSERT_TRUE(g1);

    g1->addMember(*i1);

    EXPECT_EQ(1, g1->memberCount());

    Gd::Nm::ObjectIterator it = g1->members();
    EXPECT_TRUE(i1 == it.next());
    EXPECT_TRUE(NULL == it.next());
}

