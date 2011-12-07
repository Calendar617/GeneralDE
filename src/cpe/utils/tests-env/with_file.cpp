#include <stdlib.h>
#include "cpe/utils/tests-env/with_file.hpp"

namespace utils { namespace testenv {

void with_file::SetUp() {
    char * name_buf = t_strdup("/tmp/gtest.XXXXXX");
    EXPECT_TRUE(name_buf);
    m_path_base = mkdtemp(name_buf);
}

void with_file::TearDown() {
    if (m_path_base) {
        dir_rm_recursion(m_path_base, NULL, NULL);
    }
}

const char * 
with_file::t_dir_base(void) {
    return m_path_base;
}

char *
with_file::t_path_make(const char * subpath) {
    int len = strlen(subpath);
    int baselen = strlen(m_path_base);

    char * buf = (char*)t_alloc(baselen + len + 1 + 1);
    memcpy(buf, m_path_base, baselen);
    memcpy(buf + baselen, "/", 1);
    memcpy(buf + baselen + 1, subpath, len);
    buf[len + baselen + 1] = 0;
    return buf;
}

}}

