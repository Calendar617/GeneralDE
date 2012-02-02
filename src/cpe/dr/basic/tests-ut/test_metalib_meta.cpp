#include "gtest/gtest.h"
#include "cpe/dr/dr_metalib_manage.h"
#include "cpe/dr/dr_ctypes_info.h"
#include "with_InputMetaLibTest.hpp"

struct MetaData {
    const char * name;
    const char * desc;
    int type;
    size_t size;
    int entry_num;
    int current_version;
    int base_version;
};

class MetaLibManagerMetaTest : public ::testing::TestWithParam<MetaData>, public WithInputMetaLibTest {
    virtual void SetUp() {
        loadLib();
    }

    virtual void TearDown() {
        freeLib();
    }
};

TEST_P(MetaLibManagerMetaTest, CheckData) {
    MetaData meta = GetParam();

    LPDRMETA pMeta = dr_lib_find_meta_by_name(m_lib, meta.name);
    ASSERT_TRUE(pMeta) << "get meta by name " << meta.name << " fail!";

    ASSERT_STREQ(meta.name, dr_meta_name(pMeta));

    ASSERT_STREQ(meta.desc, dr_meta_desc(pMeta)) << meta.name << " desc error!";

    ASSERT_EQ(meta.type, dr_meta_type(pMeta)) << meta.name << " type error!";

    ASSERT_EQ(meta.size, dr_meta_size(pMeta)) << meta.name << " size error!";

    ASSERT_EQ(meta.entry_num, dr_meta_entry_num(pMeta)) << meta.name << " entry_num error!";

    ASSERT_EQ(meta.base_version, dr_meta_based_version(pMeta)) << meta.name << " based_version error!";

    ASSERT_EQ(meta.current_version, dr_meta_current_version(pMeta)) << meta.name << " current_version error!";
}

MetaData metaDataCases[] = {
    /*  name       desc             type                size entry_num   current_version  base_version*/
      {"PkgHead",  "PkgHead.desc",  CPE_DR_TYPE_STRUCT, 20,  6,          1,               1}
    , {"CmdLogin", "",              CPE_DR_TYPE_STRUCT, 200, 3,          10,              10}
    , {"CmdLogout","",              CPE_DR_TYPE_STRUCT, 136, 3,          10,              10}
    , {"PkgBody",  "",              CPE_DR_TYPE_UNION,  200, 2,          10,              10}
    , {"Pkg",      "",              CPE_DR_TYPE_STRUCT, 220, 2,          10,              10}
};

INSTANTIATE_TEST_CASE_P(
    CheckAllMetaData,
    MetaLibManagerMetaTest,
    testing::ValuesIn(metaDataCases));
