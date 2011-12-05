#ifndef CPE_DR_TEST_FILETEST_H
#define CPE_DR_TEST_FILETEST_H
#include "gtest/gtest.h"
#include "cpe/utils/error_list.h"
#include "cpe/utils/file.h"
#include "cpe/utils/buffer.h"

class FileTest : public ::testing::Test {
public:
    FileTest();

    virtual void SetUp();
    virtual void TearDown();

    error_list_t m_el;
    struct error_monitor m_em;
    struct error_monitor_node m_printer;
    const char * m_base;
    struct mem_buffer m_buffer;

    char * make_path_name(const char * sub);
};

#endif

