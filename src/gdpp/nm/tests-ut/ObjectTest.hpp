#ifndef GDPP_NM_TEST_OBJECTTEST_H
#define GDPP_NM_TEST_OBJECTTEST_H
#include "gd/nm/tests-env/with_nm.hpp"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/nm/Object.hpp"

typedef LOKI_TYPELIST_1(gd::nm::testenv::with_nm) ObjectTestBase;

class ObjectTest : public testenv::fixture<ObjectTestBase> {
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

    int _destoryCount;
};

#endif
