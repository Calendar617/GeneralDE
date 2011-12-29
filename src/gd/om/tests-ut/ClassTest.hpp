#ifndef CPE_OM_TEST_CLASSTEST_H
#define CPE_OM_TEST_CLASSTEST_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gd/om/om_error.h"
#include "../om_class_i.h"

typedef LOKI_TYPELIST_1(
    utils::testenv::with_em) ClassTestBase;

class ClassTest : public testenv::fixture<ClassTestBase> {
public:
    virtual void SetUp();
    virtual void TearDown();

    gd_om_class_id_t addClass(
        const char * className,
        size_t object_size,
        size_t page_size,
        size_t align);

    struct gd_om_class_mgr m_classMgr;
};

#endif
