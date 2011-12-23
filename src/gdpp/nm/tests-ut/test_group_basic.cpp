#include "GroupTest.hpp"

TEST_F(GroupTest, create_basic) {
    TestGroup * o = new(t_nm(), "object1") TestGroup(*this, 1);
    ASSERT_TRUE(o);

    EXPECT_STREQ("object1", o->name());
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
