#include "cpe/dr/dr_ctypes_info.h"
#include "cpe/dr/dr_data.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlEntryTest : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlEntryTest, entry_data) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' "
        "               desc='a1.desc'\n"
        "               cname='a1.cname'\n"
        "               type='int16'\n"
        "               id='12'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETAENTRY e = entry("PkgHead", "a1");

    EXPECT_STREQ("a1", dr_entry_name(e));
    EXPECT_STREQ("a1.desc", dr_entry_desc(e));
    EXPECT_STREQ("a1.cname", dr_entry_cname(e));
    EXPECT_EQ(1, dr_entry_version(e));
    EXPECT_EQ(CPE_DR_TYPE_INT16, dr_entry_type(e));
    EXPECT_EQ(NULL, dr_entry_dft_value(e));
}

TEST_F(BuildFromXmlEntryTest, no_name) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry type='int16'\n/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA head = meta("PkgHead");

    EXPECT_EQ(0, dr_meta_entry_num(head));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_META_NO_NAME));
}

TEST_F(BuildFromXmlEntryTest, no_type) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1'\n/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA head = meta("PkgHead");

    EXPECT_EQ(0, dr_meta_entry_num(head));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_ENTRY_NO_TYPE));
}

TEST_F(BuildFromXmlEntryTest, type_composite) {
    EXPECT_EQ(
        0,
        parseMeta(
            "<metalib tagsetversion='1' name='net'  version='10'>"
            "    <struct name='A1' version='1'>"
            "	     <entry name='a1' type='int32'\n/>"
            "    </struct>"
            "    <struct name='A2' version='1'>"
            "	     <entry name='a1' type='A1'\n/>"
            "    </struct>"
            "</metalib>"
            ));

    LPDRMETA m = meta("A2");
    ASSERT_TRUE(m);

    EXPECT_EQ(1, dr_meta_entry_num(m));
}

TEST_F(BuildFromXmlEntryTest, version_new) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' version='2'\n type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA head = meta("PkgHead");

    EXPECT_EQ(1, dr_meta_based_version(head));
    EXPECT_EQ(2, dr_meta_current_version(head));
}

TEST_F(BuildFromXmlEntryTest, version_bigger) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' version='11'\n type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA head = meta("PkgHead");

    EXPECT_EQ(0, dr_meta_entry_num(head));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_INVALID_VERSION));
}

TEST_F(BuildFromXmlEntryTest, size_for_string) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='string' size='12'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ((size_t)12, dr_entry_size(entry("PkgHead", "a1")));
    EXPECT_EQ((size_t)12, dr_meta_size(meta("PkgHead")));
}

TEST_F(BuildFromXmlEntryTest, size_for_int) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='int16' size='12'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ((size_t)2, dr_entry_size(entry("PkgHead", "a1")));
}

TEST_F(BuildFromXmlEntryTest, size_for_complex_type) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='U1' version='1'>"
        "	     <entry name='a1' type='int16'/>"
        "	     <entry name='a1' type='int16'/>"
        "    </struct>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='U1' size='12'/>"
        "    </struct>"
        "</metalib>"
        );

    EXPECT_EQ((size_t)4, dr_entry_size(entry("PkgHead", "a1")));
}

TEST_F(BuildFromXmlEntryTest, string_no_size) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='string'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA head = meta("PkgHead");

    EXPECT_EQ(0, dr_meta_entry_num(head));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE));
}

TEST_F(BuildFromXmlEntryTest, dftvalue_basic) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' defaultvalue='12' type='int16'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETAENTRY e = entry("PkgHead", "a1");
    ASSERT_TRUE(e);

    const void * dftValue = dr_entry_dft_value(e);
    ASSERT_TRUE(dftValue) << "dftValue not exist";

    EXPECT_EQ(13, dr_read_int32(dftValue, e));
}

TEST_F(BuildFromXmlEntryTest, string_def_value) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='string' size='5' defaultvalue='abc'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETAENTRY e = entry("PkgHead", "a1");
    ASSERT_TRUE(e);

    const void * dftValue = dr_entry_dft_value(e);
    ASSERT_TRUE(dftValue) << "dftValue not exist";

    EXPECT_STREQ("abc", (const char *)dftValue);
}

TEST_F(BuildFromXmlEntryTest, string_def_value_overflow) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' type='string' size='5' defaultvalue='abcdef'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETAENTRY e = entry("PkgHead", "a1");
    ASSERT_TRUE(e);

    const void * dftValue = dr_entry_dft_value(e);
    ASSERT_TRUE(dftValue) << "dftValue not exist";

    EXPECT_STREQ("abcd", (const char *)dftValue);
    EXPECT_TRUE(haveError(CPE_DR_ERROR_ENTRY_INVALID_SIZE_VALUE));
}
