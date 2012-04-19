#ifndef GDPP_UTILS_TEST_IDGENERATOR_H
#define GDPP_UTILS_TEST_IDGENERATOR_H
#include "cpe/utils/tests-env/test-fixture.hpp"
#include "cpe/utils/tests-env/with_file.hpp"
#include "cpe/utils/tests-env/with_em.hpp"
#include "gd/app/tests-env/with_app.hpp"
#include "gd/utils/id_generator.h"

typedef LOKI_TYPELIST_3(
    utils::testenv::with_em
    , utils::testenv::with_file
    , gd::app::testenv::with_app
    ) IdGeneratorTestBase;

class IdGeneratorTest : public testenv::fixture<IdGeneratorTestBase> {
public:
    virtual void SetUp();

    void write_in_file(const char * value);

    void set_load_from_file(void);

    gd_id_generator_t id_generator(void);
};

#endif
