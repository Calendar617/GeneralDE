#include "cpe/dr/dr_ctypes_info.h"
#include "BuildFromXmlTest.hpp"

class BuildFromXmlEntryTest : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlEntryTest, entry_data) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead'>"
        "	     <entry name='a1' "
        "               desc='a1.desc'\n"
        "               cname='a1.cname'\n"
        "               type='smallint'\n"
        "               defaultvalue='1'\n"
        "               id='12'/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, "PkgHead");
    ASSERT_TRUE(meta) << "get meta fail!";

    LPDRMETAENTRY entry = dr_get_entryptr_by_name(meta, "a1");
    ASSERT_TRUE(entry) << "get entry fail!";

    EXPECT_STREQ("a1", dr_get_entry_name(entry));
    EXPECT_STREQ("a1.desc", dr_get_entry_desc(entry));
    EXPECT_STREQ("a1.cname", dr_get_entry_cname(entry));
}

TEST_F(BuildFromXmlEntryTest, no_name) {
    parseMeta(
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <struct name='PkgHead'>"
        "	     <entry type='smallint'\n/>"
        "    </struct>"
        "</metalib>"
        );

    LPDRMETA meta = dr_get_meta_by_name(m_metaLib, "PkgHead");
    ASSERT_TRUE(meta) << "get meta fail!";

    EXPECT_EQ(0, dr_get_entry_num(meta));
    EXPECT_TRUE(haveError(CPE_DR_ERROR_META_NO_NAME));
}
