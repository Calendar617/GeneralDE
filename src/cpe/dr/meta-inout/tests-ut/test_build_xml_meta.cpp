#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlMetaTest : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlMetaTest, struct_data) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='PkgHead' desc='PkgHead.desc' version='1' id='33'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "</metalib>"
            ));

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_get_meta_name(meta));
    EXPECT_STREQ("PkgHead.desc", dr_get_meta_desc(meta));
    EXPECT_EQ(CPE_DR_TYPE_STRUCT, dr_get_meta_type(meta));
    EXPECT_EQ(33, dr_get_meta_id(meta));
    EXPECT_EQ(1, dr_get_meta_based_version(meta));
    EXPECT_EQ(1, dr_get_meta_current_version(meta));
    EXPECT_EQ(1, dr_get_entry_num(meta));
    EXPECT_EQ(1, dr_get_meta_align(meta));

    int entryPos = -1;
    EXPECT_EQ(0, dr_get_entry_by_name(&entryPos, meta, "a1"));
    EXPECT_EQ(0, entryPos);
}

TEST_F(BuildFromXmlMetaTest, struct_no_entry) {
    EXPECT_EQ(
        CPE_DR_ERROR_META_NO_ENTRY,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='PkgHead' desc='PkgHead.desc' version='1' id='33'/>"
            "</metalib>"
            ));
}

TEST_F(BuildFromXmlMetaTest, union_data) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <union name='PkgHead' desc='PkgHead.desc' version='1' id='33'>"
            "	     <entry name='a1' type='int8'/>"
            "    </union>"
            "</metalib>"
            ));

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_get_meta_name(meta));
    EXPECT_STREQ("PkgHead.desc", dr_get_meta_desc(meta));
    EXPECT_EQ(CPE_DR_TYPE_UNION, dr_get_meta_type(meta));
    EXPECT_EQ(1, dr_get_meta_based_version(meta));
    EXPECT_EQ(1, dr_get_meta_current_version(meta));
    EXPECT_EQ(1, dr_get_entry_num(meta));
}

TEST_F(BuildFromXmlMetaTest, enum_no_entry) {
    EXPECT_EQ(
        CPE_DR_ERROR_META_NO_ENTRY,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <union name='PkgHead' desc='PkgHead.desc' version='1' id='33'/>"
            "</metalib>"
            ));
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

TEST_F(BuildFromXmlMetaTest, no_id) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='A1' version='1'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "</metalib>"
            ));

    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A1"));
}

TEST_F(BuildFromXmlMetaTest, no_id_duplicate) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='A1' version='1'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "    <struct name='A2' version='1'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "</metalib>"
            ));

    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A2"));
}

TEST_F(BuildFromXmlMetaTest, duplicate_name) {
    EXPECT_EQ(
        CPE_DR_ERROR_META_NAME_CONFLICT,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='A1' version='1' id='3'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "    <struct name='A1' version='1' id='4'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "</metalib>"
            ));

    EXPECT_TRUE(dr_get_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_META_NAME_CONFLICT));
}

TEST_F(BuildFromXmlMetaTest, duplicate_id) {
    EXPECT_EQ(
        CPE_DR_ERROR_META_ID_CONFLICT,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='A1' version='1' id='3'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "    <struct name='A2' version='1' id='3'>"
            "	     <entry name='a1' type='int8'/>"
            "    </struct>"
            "</metalib>"
            ));

    EXPECT_TRUE(dr_get_meta_by_id(m_metaLib, 3));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_META_ID_CONFLICT));
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

TEST_F(BuildFromXmlMetaTest, composite_basic) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='1'>"
            "    <struct name='S' version='1'>"
            "	     <entry name='a1' type='int16'/>"
            "    </struct>"
            "    <struct name='S2' version='1'>"
            "	     <entry name='m_s' type='S'/>"
            "	     <entry name='a2' type='int16'/>"
            "    </struct>"
            "</metalib>"
            ));

    LPDRMETA metaS = dr_get_meta_by_name(m_metaLib, "S");
    ASSERT_TRUE(metaS);

    LPDRMETA metaS2 = dr_get_meta_by_name(m_metaLib, "S2");
    ASSERT_TRUE(metaS2);
    ASSERT_TRUE(dr_get_entryptr_by_name(metaS2, "m_s"));
    EXPECT_TRUE(metaS == dr_get_entry_ref_type(dr_get_entryptr_by_name(metaS2, "m_s")));
}
