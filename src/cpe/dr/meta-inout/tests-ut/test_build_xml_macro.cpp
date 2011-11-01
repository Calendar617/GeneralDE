#include "BuildFromXmlTest.hpp"

class BuildFromXmlMacroTest : public BuildFromXmlTest {
};

TEST_F(BuildFromXmlMacroTest, macro_num) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <macro name='VERSION' value='100' desc='VERSION.desc'/>"
        "    <macro name='MAX_BODY_LEN' value='32000' />"
        "</metalib>"
        );

    ASSERT_EQ(
        2,
        dr_get_metalib_macro_num(m_metaLib));
}

TEST_F(BuildFromXmlMacroTest, macro_order) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <macro name='VERSION' value='100' desc='VERSION.desc'/>"
        "    <macro name='MAX_BODY_LEN' value='32000' />"
        "</metalib>"
        );

    LPDRMACRO macro0 = dr_get_metalib_macro_by_index(m_metaLib, 0);
    ASSERT_TRUE(macro0);
    LPDRMACRO macro1 = dr_get_metalib_macro_by_index(m_metaLib, 1);
    ASSERT_TRUE(macro1);

    EXPECT_STREQ("VERSION", dr_get_macro_name_by_ptr(m_metaLib, macro0));
    EXPECT_STREQ("MAX_BODY_LEN", dr_get_macro_name_by_ptr(m_metaLib, macro1));
}

TEST_F(BuildFromXmlMacroTest, macro_data) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <macro name='VERSION' value='100' desc='VERSION.desc'/>"
        "</metalib>"
        );


    LPDRMACRO macro0 = dr_get_metalib_macro_by_index(m_metaLib, 0);
    ASSERT_TRUE(macro0);
    EXPECT_STREQ("VERSION.desc", dr_get_macro_desc_by_ptr(m_metaLib, macro0));

    int value0 = -1;
    ASSERT_EQ(0, dr_get_macro_value_by_ptr(&value0, macro0)) << "call dr_get_macro_value_by_ptr fail!";
    EXPECT_EQ(100, value0);
}

TEST_F(BuildFromXmlMacroTest, macro_no_value) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <macro name='VERSION'/>"
        "</metalib>"
        );

    ASSERT_EQ(0, dr_get_metalib_macro_num(m_metaLib));
    ASSERT_TRUE(haveError(CPE_DR_ERROR_MACRO_NO_VALUE));
}

TEST_F(BuildFromXmlMacroTest, macro_no_name) {
    parseMeta(
        "<?xml version='1.0' standalone='yes' ?>"
        "<metalib tagsetversion='1' name='net'  version='10'>"
        "    <macro value='100'/>"
        "</metalib>"
        );

    ASSERT_EQ(0, dr_get_metalib_macro_num(m_metaLib));
    ASSERT_TRUE(haveError(CPE_DR_ERROR_MACRO_NO_NAME_ATTR));
}
