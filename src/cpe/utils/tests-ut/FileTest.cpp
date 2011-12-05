#include "FileTest.hpp"

FileTest::FileTest()
    : m_el(NULL)
    , m_base("dir-test")
{
}

void FileTest::SetUp() {
    m_el = cpe_error_list_create(NULL);
    cpe_error_monitor_init(&m_em, cpe_error_list_collect, m_el);
    cpe_error_monitor_node_init(&m_printer, cpe_error_log_to_consol, NULL);
    cpe_error_monitor_add_node(&m_em, &m_printer);

    CPE_DEF_ERROR_MONITOR(tem, cpe_error_log_to_consol, NULL);

    dir_rm_recursion(m_base, &tem, NULL);
    EXPECT_EQ(0, dir_mk_recursion(m_base, DIR_DEFAULT_MODE, &tem, NULL));

    mem_buffer_init(&m_buffer, NULL);
}

void FileTest::TearDown() {
    cpe_error_list_free(m_el);
    m_el = NULL;

    CPE_DEF_ERROR_MONITOR(tem, cpe_error_log_to_consol, NULL);
    EXPECT_EQ(0, dir_rm_recursion(m_base, &tem, NULL));

    mem_buffer_clear(&m_buffer);
}

char * FileTest::make_path_name(const char * sub) {
    int baseLen = strlen(m_base);
    int subLen = strlen(sub);

    char * r = (char *)mem_buffer_alloc(&m_buffer, baseLen + subLen + 1 + 1);

    memcpy(r, m_base, baseLen);
    r[baseLen] = '/';
    memcpy(r + baseLen + 1, sub, subLen + 1);

    return r;
}
