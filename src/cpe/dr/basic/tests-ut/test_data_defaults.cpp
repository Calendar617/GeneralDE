#include "gtest/gtest.h"
#include "cpe/utils/buffer.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_data.h"
#include "with_InputMetaLibTest.hpp"

class DataDefaultsTest : public ::testing::Test, public WithInputMetaLibTest {
public:
    DataDefaultsTest() : m_meta(NULL), m_data(NULL) {
    }

    virtual void SetUp() {
        loadLib();
        mem_buffer_init(&m_buffer, NULL);
    }

    virtual void TearDown() {
        mem_buffer_clear(&m_buffer);
        freeLib();
    }

    struct mem_buffer m_buffer;
    LPDRMETA m_meta;
    void * m_data;

    void set_defaults(const char * metaName) {
        m_meta = dr_lib_find_meta_by_name(m_lib, metaName);
        ASSERT_TRUE(m_meta);

        m_data = mem_buffer_alloc(&m_buffer, dr_meta_size(m_meta));
        ASSERT_TRUE(m_data);

        dr_meta_set_defaults(m_data, m_meta);
    }
};

TEST_F(DataDefaultsTest, basic) {
    set_defaults("PkgHead");
    ASSERT_TRUE(m_data);

    EXPECT_EQ(13, dr_meta_read_int32(m_data, m_meta, "magic"));
    EXPECT_EQ(1, dr_meta_read_int32(m_data, m_meta, "version"));
}

TEST_F(DataDefaultsTest, no_default_numeric) {
    set_defaults("PkgHead");
    ASSERT_TRUE(m_data);

    EXPECT_EQ(0, dr_meta_read_int32(m_data, m_meta, "msgid"));
}

TEST_F(DataDefaultsTest, no_default_string) {
    set_defaults("CmdLogin");
    ASSERT_TRUE(m_data);

    EXPECT_STREQ("", dr_meta_read_string(m_data, m_meta, "msgid"));
}
