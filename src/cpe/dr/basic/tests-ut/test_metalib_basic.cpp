#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_init.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "with_InputMetaLibTest.hpp"

class MetalibManagerTest : public ::testing::Test, public WithInputMetaLibTest {
    virtual void SetUp() {
        loadLib();
    }

    virtual void TearDown() {
        freeLib();
    }
};

TEST_F(MetalibManagerTest, Size) {
    ASSERT_EQ((size_t)m_libSize, dr_lib_size(m_lib));
}

TEST_F(MetalibManagerTest, Name) {
    ASSERT_STREQ("net", dr_lib_name(m_lib));
}

TEST_F(MetalibManagerTest, Version) {
    ASSERT_EQ(10, dr_lib_version(m_lib));
}

TEST_F(MetalibManagerTest, BuildVersion) {
    ASSERT_EQ(11, dr_lib_build_version(m_lib));
}

TEST_F(MetalibManagerTest, GetMicroNum) {
    ASSERT_EQ(7, dr_lib_macro_num(m_lib));
}

TEST_F(MetalibManagerTest, GetMicroByIdx_Overflow) {
    ASSERT_TRUE(dr_lib_macro_at(m_lib, -1) == 0);
    ASSERT_TRUE(dr_lib_macro_at(m_lib, 7) == 0);
}

TEST_F(MetalibManagerTest, GetMicroValueExist) {
    int checkValue = -1;
    ASSERT_EQ(0, dr_lib_find_macro_value(&checkValue, m_lib, "VERSION"));

    ASSERT_EQ(100, checkValue);
}

TEST_F(MetalibManagerTest, GetMicroValueNotExist) {
    int checkValue = -1;
    ASSERT_LT(dr_lib_find_macro_value(&checkValue, m_lib, "NotExistMacro"), 0);

    ASSERT_EQ(-1, checkValue);
}

TEST_F(MetalibManagerTest, GetMetaByNameExist) {
    LPDRMETA lpMeta = dr_lib_find_meta_by_name(m_lib, "PkgBody");
    ASSERT_TRUE(lpMeta != 0) << "dr_find_meta_by_name(PkgBody) fail";

    ASSERT_STREQ("PkgBody", dr_meta_name(lpMeta));
}

TEST_F(MetalibManagerTest, GetMetaByNameNotExist) {
    ASSERT_EQ(NULL, dr_lib_find_meta_by_name(m_lib, "NotExistEntry")) << "dr_find_meta_by_name(NotExistEntry) fail";
}

TEST_F(MetalibManagerTest, GetMetaByIdExist) {
    LPDRMETA lpMeta = dr_lib_find_meta_by_id(m_lib, 1);
    ASSERT_TRUE(lpMeta != 0) << "dr_get_meta_by_id(1) fail";

    ASSERT_STREQ("PkgHead", dr_meta_name(lpMeta));
}

TEST_F(MetalibManagerTest, GetMetaByIdNotExist) {
    ASSERT_EQ(NULL, dr_lib_find_meta_by_id(m_lib, 34)) << "dr_get_meta_by_id(34) should not exist";
}

TEST_F(MetalibManagerTest, GetMetaByIdNegative) {
    ASSERT_EQ(NULL, dr_lib_find_meta_by_id(m_lib, -1)) << "dr_get_meta_by_id(-1) should not exist";
}

