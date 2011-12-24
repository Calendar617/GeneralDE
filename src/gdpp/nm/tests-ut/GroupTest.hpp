#ifndef GDPP_NM_TEST_GROUPTEST_H
#define GDPP_NM_TEST_GROUPTEST_H
#include "gdpp/nm/Group.hpp"
#include "ObjectTest.hpp"

typedef Loki::NullType GroupTestBase;

class GroupTest : public testenv::fixture<GroupTestBase, ObjectTest> {
public:
    GroupTest();

    class TestGroup : public Gd::Nm::Group {
    public:
        TestGroup(GroupTest & t, int v) : _t(t), _value(v) {
        }

        virtual ~TestGroup() {
            ++_t._destoryCount;
        }

        GroupTest & _t;
        int _value;
    };
};

#endif
