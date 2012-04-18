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

    LPDRMETA head = meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_meta_name(head));
    EXPECT_STREQ("PkgHead.desc", dr_meta_desc(head));
    EXPECT_EQ(CPE_DR_TYPE_STRUCT, dr_meta_type(head));
    EXPECT_EQ(33, dr_meta_id(head));
    EXPECT_EQ(1, dr_meta_based_version(head));
    EXPECT_EQ(1, dr_meta_current_version(head));
    EXPECT_EQ(1, dr_meta_entry_num(head));
    EXPECT_EQ(1, dr_meta_align(head));

    EXPECT_EQ(0, dr_meta_find_entry_idx_by_name(head, "a1"));
}

TEST_F(BuildFromXmlMetaTest, id_use_macro) {
    EXPECT_EQ(
        CPE_DR_ERROR_META_NO_ENTRY,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <macro name='macro_1' value='100'/>"
            "    <struct name='PkgHead' desc='PkgHead.desc' version='1' id='macro_1'/>"
            "</metalib>"
            ));

    LPDRMETA head = meta("PkgHead");
    EXPECT_EQ(100, dr_meta_id(head));
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

    LPDRMETA head = meta("PkgHead");

    EXPECT_STREQ("PkgHead", dr_meta_name(head));
    EXPECT_STREQ("PkgHead.desc", dr_meta_desc(head));
    EXPECT_EQ(CPE_DR_TYPE_UNION, dr_meta_type(head));
    EXPECT_EQ(1, dr_meta_based_version(head));
    EXPECT_EQ(1, dr_meta_current_version(head));
    EXPECT_EQ(1, dr_meta_entry_num(head));
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

    EXPECT_TRUE(meta("A1") != NULL) << "A1 should exist!";
    EXPECT_TRUE(meta("A2") != NULL) << "A2 should exist!";

    //now change input order

    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A2' version='1'/>"
        "    <struct name='A1' version='1'/>"
        "</metalib>"
        );

    EXPECT_TRUE(meta("A1") != NULL) << "A1 should exist!";
    EXPECT_TRUE(meta("A2") != NULL) << "A2 should exist!";
}

TEST_F(BuildFromXmlMetaTest, by_id) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1' id='1'/>"
        "    <struct name='A2' version='1' id='2'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_lib_find_meta_by_id(m_metaLib, 1) != NULL) << "id 1 should exist!";
    EXPECT_TRUE(dr_lib_find_meta_by_id(m_metaLib, 2) != NULL) << "id 2 should exist!";

    //now change input order

    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A2' version='1' id='2'/>"
        "    <struct name='A1' version='1' id='1'/>"
        "</metalib>"
        );

    EXPECT_TRUE(dr_lib_find_meta_by_id(m_metaLib, 1) != NULL) << "id 1 should exist!";
    EXPECT_TRUE(dr_lib_find_meta_by_id(m_metaLib, 2) != NULL) << "id 2 should exist!";
}

TEST_F(BuildFromXmlMetaTest, version_not_exist) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1'/>"
        "</metalib>"
        );

    EXPECT_FALSE(dr_lib_find_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(t_em_have_errno(CPE_DR_ERROR_NO_VERSION));
}

TEST_F(BuildFromXmlMetaTest, version_bigger) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='11'/>"
        "</metalib>"
        );

    EXPECT_FALSE(dr_lib_find_meta_by_name(m_metaLib, "A1"));
    EXPECT_TRUE(t_em_have_errno(CPE_DR_ERROR_INVALID_VERSION));
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

    EXPECT_TRUE(meta("A1"));
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

    EXPECT_TRUE(meta("A1"));
    EXPECT_TRUE(meta("A2"));
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

    EXPECT_TRUE(meta("A1"));
    EXPECT_TRUE(t_em_have_errno(CPE_DR_ERROR_META_NAME_CONFLICT));
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

    EXPECT_TRUE(dr_lib_find_meta_by_id(m_metaLib, 3));
    EXPECT_TRUE(t_em_have_errno(CPE_DR_ERROR_META_ID_CONFLICT));
}

TEST_F(BuildFromXmlMetaTest, set_align) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='A1' version='1' id='3' align='3'/>"
        "</metalib>"
        );

    EXPECT_EQ(3, dr_meta_align(meta("A1")));
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

    ASSERT_TRUE(t_em_no_error());

    LPDRMETA metaS = meta("S");
    ASSERT_TRUE(metaS);

    LPDRMETA metaS2 = meta("S2");
    ASSERT_TRUE(metaS2);
    ASSERT_TRUE(dr_meta_find_entry_by_name(metaS2, "m_s"));
    EXPECT_TRUE(metaS == dr_entry_ref_meta(dr_meta_find_entry_by_name(metaS2, "m_s")));
}
