#ifndef GDPP_NM_TEST_REQTEST_H
#define GDPP_NM_TEST_REQTEST_H
#include "gd/nm/tests-env/with_nm.hpp"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/nm/Group.hpp"

typedef LOKI_TYPELIST_1(gd::nm::testenv::with_nm) GroupTestBase;

class GroupTest : public testenv::fixture<GroupTestBase> {
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

    int _destoryCount;
};

#endif
