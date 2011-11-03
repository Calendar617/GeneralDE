#include "cpe/dr/dr_ctypes_info.h"
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
        "               type='smallint'\n"
        "               defaultvalue='1'\n"
        "               id='12'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETAENTRY entry = get_entry("PkgHead", "a1");

    EXPECT_STREQ("a1", dr_get_entry_name(entry));
    EXPECT_STREQ("a1.desc", dr_get_entry_desc(entry));
    EXPECT_STREQ("a1.cname", dr_get_entry_cname(entry));
    EXPECT_EQ(1, dr_get_entry_version(entry));
    EXPECT_EQ(CPE_DR_TYPE_SMALLINT, dr_get_entry_type(entry));
}

TEST_F(BuildFromXmlEntryTest, no_name) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry type='smallint'\n/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_EQ(0, dr_get_entry_num(meta));
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

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_EQ(0, dr_get_entry_num(meta));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_ENTRY_NO_TYPE));
}

TEST_F(BuildFromXmlEntryTest, version_new) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' version='2'\n type='smallint'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_EQ(1, dr_get_meta_based_version(meta));
    EXPECT_EQ(2, dr_get_meta_current_version(meta));
}

TEST_F(BuildFromXmlEntryTest, version_bigger) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead' version='1'>"
        "	     <entry name='a1' version='11'\n type='smallint'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA meta = get_meta("PkgHead");

    EXPECT_EQ(0, dr_get_entry_num(meta));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_INVALID_VERSION));
}
