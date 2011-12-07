#ifndef CPE_UTILS_WITH_FILE_H
#define CPE_UTILS_WITH_FILE_H
#include "../file.h"
#include "test-env.hpp"

namespace utils { namespace testenv {

class with_file : public ::testenv::env<> {
public:
    void SetUp();
    void TearDown();

    const char * t_dir_base(void);
    char * t_path_make(const char * subpath);
private:
    const char * m_path_base;
};

}}

#endif

