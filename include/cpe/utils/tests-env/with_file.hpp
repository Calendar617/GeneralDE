#ifndef CPE_UTILS_TEST_FIXTURE_H
#define CPE_UTILS_TEST_FIXTURE_H
#include "../file.h"
#include "test-env.hpp"

class with_file : public env_gen<> {
public:
    void SetUp();
    void TearDown();

    const char * path_base(void);
    const char * path_make(const char *);

private:
    const char * m_path_base;
};

#endif

