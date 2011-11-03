#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlMetaTest : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlMetaTest, struct_data) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' desc='PkgHead.desc' version='1' id='33'/>"
        "</metalib>"
        );

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_get_meta_name(meta));
    EXPECT_STREQ("PkgHead.desc", dr_get_meta_desc(meta));
    EXPECT_EQ(CPE_DR_TYPE_STRUCT, dr_get_meta_type(meta));
    EXPECT_EQ(33, dr_get_meta_id(meta));
    EXPECT_EQ(1, dr_get_meta_based_version(meta));
    EXPECT_EQ(1, dr_get_meta_current_version(meta));
    EXPECT_EQ(0, dr_get_entry_num(meta));
    EXPECT_EQ(1, dr_get_meta_align(meta));
}

TEST_F(BuildFromXmlMetaTest, union_data) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <union name='PkgHead' desc='PkgHead.desc' version='1'>"
        "    </union>"
        "</metalib>"
        );

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_get_meta_name(meta));
    EXPECT_STREQ("PkgHead.desc", dr_get_meta_desc(meta));
    EXPECT_EQ(CPE_DR_TYPE_UNION, dr_get_meta_type(meta));
    EXPECT_EQ(1, dr_get_meta_based_version(meta));
    EXPECT_EQ(1, dr_get_meta_current_version(meta));
    EXPECT_EQ(0, dr_get_entry_num(meta));
}

TEST_F(BuildFromXmlMetaTest, by_name) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1'/>"
        "    <struct name='A2' version='1'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A1") != NULL) << "A1 should exist!";
    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A2") != NULL) << "A2 should exist!";

    //now change input order

    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A2' version='1'/>"
        "    <struct name='A1' version='1'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A1") != NULL) << "A1 should exist!";
    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A2") != NULL) << "A2 should exist!";
}

TEST_F(BuildFromXmlMetaTest, by_id) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1' id='1'/>"
        "    <struct name='A2' version='1' id='2'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_get_meta_by_id(m_metaLib, 1) != NULL) << "id 1 should exist!";
    EXPECT_TRUE(dr_get_meta_by_id(m_metaLib, 2) != NULL) << "id 2 should exist!";

    //now change input order

    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A2' version='1' id='2'/>"
        "    <struct name='A1' version='1' id='1'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_get_meta_by_id(m_metaLib, 1) != NULL) << "id 1 should exist!";
    EXPECT_TRUE(dr_get_meta_by_id(m_metaLib, 2) != NULL) << "id 2 should exist!";
}

TEST_F(BuildFromXmlMetaTest, version_not_exist) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1'/>"
        "</metalib>"
        );

    EXPECT_FALSE(dr_get_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_NO_VERSION));
}

TEST_F(BuildFromXmlMetaTest, version_bigger) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='11'/>"
        "</metalib>"
        );

    EXPECT_FALSE(dr_get_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_INVALID_VERSION));
}

TEST_F(BuildFromXmlMetaTest, duplicate_name) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1'/>"
        "    <struct name='A1' version='1'/>"
        "</metalib>"
        );

    ASSERT_TRUE(dr_get_meta_by_name(m_metaLib, "A1"));
    ASSERT_TRUE(haveError(CPE_DR_ERROR_META_NAME_CONFLICT));
}

TEST_F(BuildFromXmlMetaTest, duplicate_id) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1' id='3'/>"
        "    <struct name='A2' version='1' id='3'/>"
        "</metalib>"
        );

    ASSERT_TRUE(dr_get_meta_by_id(m_metaLib, 3));
    ASSERT_TRUE(haveError(CPE_DR_ERROR_META_ID_CONFLICT));
}

TEST_F(BuildFromXmlMetaTest, set_align) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1' id='3' align='3'/>"
        "</metalib>"
        );

    EXPECT_EQ(3, dr_get_meta_align(get_meta("A1")));
}
