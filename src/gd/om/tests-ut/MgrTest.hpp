#ifndef CPE_OM_TEST_MGRTEST_H
#define CPE_OM_TEST_MGRTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gd/om/om_error.h"
#include "gd/om/om_manage.h"
#include "gd/om/om_class.h"
#include "gd/om/om_object.h"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) MgrTestBase;

class MgrTest : public testenv::fixture<MgrTestBase> {
public:
    MgrTest();

    virtual void SetUp();
    virtual void TearDown();

    gd_om_class_id_t addClass(const char * className, size_t object_size);
    gd_om_oid_t obj_alloc(cpe_hash_string_t className);

    static gd_om_buffer_id_t buf_alloc(size_t size, void * context);

    gd_om_mgr_t m_omm;
};

#endif
