#ifndef GDPP_NM_TEST_MANAGERTEST_H
#define GDPP_NM_TEST_MANAGERTEST_H
#include "gd/nm/tests-env/with_nm.hpp"
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gdpp/nm/Object.hpp"
#include "gdpp/nm/Group.hpp"
#include "gdpp/nm/Manager.hpp"

typedef LOKI_TYPELIST_1(gd::nm::testenv::with_nm) NmTestBase;

class NmTest : public testenv::fixture<NmTestBase> {
public:
    NmTest();

    class TestObject : public Gd::Nm::Object {
    public:
        TestObject(NmTest & t, int v) : _t(t), _value(v) {
        }

        virtual ~TestObject() {
            ++_t._destoryCount;
        }

        NmTest & _t;
        int _value;
    };

    class TestGroup : public Gd::Nm::Group {
    public:
        TestGroup(NmTest & t, int v) : _t(t), _value(v) {
        }

        virtual ~TestGroup() {
            ++_t._destoryCount;
        }

        NmTest & _t;
        int _value;
    };

    Gd::Nm::Manager & mgr(void) { return *((Gd::Nm::Manager*)t_nm()); }
    Gd::Nm::Manager const & mgr_const(void) { return mgr(); }

    int _destoryCount;
};

#endif
