#ifndef GDPP_NM_TEST_OBJECTTEST_H
#define GDPP_NM_TEST_OBJECTTEST_H
#include "ManagerTest.hpp"

typedef Loki::NullType ObjectTestBase;

class ObjectTest : public testenv::fixture<ObjectTestBase, ManagerTest> {
public:
    ObjectTest();

    class TestObject : public Gd::Nm::Object {
    public:
        TestObject(ObjectTest & t, int v) : _t(t), _value(v) {
        }

        virtual ~TestObject() {
            ++_t._destoryCount;
        }

        ObjectTest & _t;
        int _value;
    };
};

#endif
